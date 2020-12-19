#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define PANIC(...) \
    fprintf(stderr, __VA_ARGS__); \
    exit(EXIT_FAILURE);

typedef enum TokenType {
    TOKEN_MUL,
    TOKEN_ADD,
    TOKEN_INT,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END_OF_LINE,
    TOKEN_END_OF_FILE
} TokenType;

typedef struct Token {
    TokenType type;
    int value;
} Token;

Token next_token(FILE* file) {
    Token token;
    char ch = getc(file);
    while (ch == ' ') ch = getc(file);
    switch (ch) {
        case EOF: token.type = TOKEN_END_OF_FILE; break;
        case '\n': token.type = TOKEN_END_OF_LINE; break;
        case '(': token.type = TOKEN_LPAREN; break;
        case ')': token.type = TOKEN_RPAREN; break;
        case '*': token.type = TOKEN_MUL; break;
        case '+': token.type = TOKEN_ADD; break;
        default:
            if (ch >= '0' && ch <= '9') {
                token.type = TOKEN_INT;
                token.value = ch - '0';
            } else {
                PANIC("Invalid character %c", ch);
            }
            break;
    }
    return token;
}

typedef enum ExpressionType {
    EXPR_TYPE_MUL,
    EXPR_TYPE_ADD,
    EXPR_TYPE_INT
} ExpressionType;

typedef struct Expression {
    ExpressionType type;
    int value;
    struct Expression *lhs;
    struct Expression *rhs;
} Expression;

void expression_free(Expression* expression) {
    if (expression->type != EXPR_TYPE_INT) {
        expression_free(expression->lhs);
        expression_free(expression->rhs);
    }
    free(expression);
}

Expression* expression_int(int value) {
    Expression* expression = (Expression*) malloc(sizeof(Expression));
    expression->type = EXPR_TYPE_INT;
    expression->value = value;
    return expression;
}

Expression* expression_binop(TokenType token_type, Expression* lhs, Expression* rhs) {
    assert(token_type == TOKEN_ADD || token_type == TOKEN_MUL);
    Expression* expression = (Expression*) malloc(sizeof(Expression));
    expression->type = token_type == TOKEN_ADD ? EXPR_TYPE_ADD : EXPR_TYPE_MUL;
    expression->lhs = lhs;
    expression->rhs = rhs;
    return expression;
}

typedef struct OperatorStack {
    TokenType op;
    struct OperatorStack *next;
} OperatorStack;

OperatorStack *operator_stack_init(TokenType op) {
    OperatorStack *stack = (OperatorStack*) malloc(sizeof(OperatorStack));
    stack->op = op;
    stack->next = NULL;
    return stack;
}

void operator_stack_push(OperatorStack **stack, TokenType op) {
    OperatorStack *head = operator_stack_init(op);
    head->next = *stack;
    *stack = head;
}

TokenType operator_stack_pop(OperatorStack **stack) {
    if (*stack == NULL) {
        PANIC("Empty stack");
    }
    OperatorStack *prev = *stack;
    *stack = (*stack)->next;
    TokenType op = prev->op;
    free(prev);
    return op;
}

TokenType operator_stack_peek(OperatorStack *stack) {
    if (stack == NULL) {
        PANIC("Empty stack");
    }
    return stack->op;
}

bool operator_stack_is_empty(OperatorStack *stack) {
    return stack == NULL;
}

typedef struct ExpressionStack {
    Expression *expression;
    struct ExpressionStack *next;
} ExpressionStack;

ExpressionStack *expression_stack_init(Expression *expression) {
    ExpressionStack *stack = (ExpressionStack*) malloc(sizeof(ExpressionStack));
    stack->expression = expression;
    stack->next = NULL;
    return stack;
}

void expression_stack_push(ExpressionStack **stack, Expression *expression) {
    ExpressionStack *head = expression_stack_init(expression);
    head->next = *stack;
    *stack = head;
}

Expression *expression_stack_pop(ExpressionStack **stack) {
    if (*stack == NULL) {
        PANIC("Empty stack");
    }
    ExpressionStack *prev = *stack;
    *stack = (*stack)->next;
    Expression *expression = prev->expression;
    free(prev);
    return expression;
}

bool expression_stack_is_empty(ExpressionStack *stack) {
    return stack == NULL;
}

void print_token(Token token) {
    switch (token.type) {
        case TOKEN_END_OF_FILE: printf("TOKEN_END_OF_FILE"); break;
        case TOKEN_END_OF_LINE: printf("TOKEN_END_OF_LINE"); break;
        case TOKEN_LPAREN: printf("TOKEN_LPAREN"); break;
        case TOKEN_RPAREN: printf("TOKEN_RPAREN"); break;
        case TOKEN_MUL: printf("TOKEN_MUL"); break;
        case TOKEN_ADD: printf("TOKEN_ADD"); break;
        case TOKEN_INT: printf("TOKEN_INT %d", token.value); break;
        default: PANIC("Unknown token type, %d", token.type);
    }
}

void print_expression(Expression *expression) {
    switch (expression->type) {
        case EXPR_TYPE_MUL:
            printf("("); print_expression(expression->lhs); printf(" * "); print_expression(expression->rhs); printf(")");
            break;
        case EXPR_TYPE_ADD:
            printf("("); print_expression(expression->lhs); printf(" + "); print_expression(expression->rhs); printf(")");
            break;
        case EXPR_TYPE_INT:
            printf("%d", expression->value);
            break;
        default: PANIC("Unknown expresssion type: %d", expression->type);
    }
}

void print_expression_tree(Expression *expression, int indent) {
    switch (expression->type) {
        case EXPR_TYPE_MUL:
            printf("%*s*\n", indent, "");
            print_expression_tree(expression->lhs, indent + 2);
            print_expression_tree(expression->rhs, indent + 2);
            break;
        case EXPR_TYPE_ADD:
            printf("%*s+\n", indent, "");
            print_expression_tree(expression->lhs, indent + 2);
            print_expression_tree(expression->rhs, indent + 2);
            break;
        case EXPR_TYPE_INT:
            printf("%*d\n", indent + 1, expression->value);
            break;
        default: PANIC("Unknown expresssion type: %d", expression->type);
    }
}

void print_expression_prefix(Expression *expression) {
    switch (expression->type) {
        case EXPR_TYPE_MUL:
            printf("(* "); print_expression_prefix(expression->lhs); printf(" "); print_expression_prefix(expression->rhs); printf(")");
            break;
        case EXPR_TYPE_ADD:
            printf("(+ "); print_expression_prefix(expression->lhs); printf(" "); print_expression_prefix(expression->rhs); printf(")");
            break;
        case EXPR_TYPE_INT:
            printf("%d", expression->value);
            break;
        default: PANIC("Unknown expresssion type: %d", expression->type);
    }
}

typedef enum Associativity {
    ASSOCIATIVITY_LEFT,
    ASSOCIATIVITY_RIGHT
} Associativity;

typedef int (*PresedenceCompareFn)(TokenType, TokenType);
typedef Associativity (*AssociativityFn) (TokenType);

Expression *next_expression(FILE* file, PresedenceCompareFn presedence_cmp, AssociativityFn associativity) {
    OperatorStack *operator_stack = NULL;
    ExpressionStack *expression_stack = NULL;

    while(true) {
        Token token = next_token(file);
        switch (token.type) {
            case TOKEN_INT:
                expression_stack_push(&expression_stack, expression_int(token.value));
                break;
            case TOKEN_ADD:
            case TOKEN_MUL:
                while (!operator_stack_is_empty(operator_stack) && operator_stack_peek(operator_stack) != TOKEN_LPAREN) {
                    int presedence = presedence_cmp(operator_stack_peek(operator_stack), token.type);
                    if (presedence > 0 || (presedence == 0 && associativity(token.type) == ASSOCIATIVITY_LEFT)) {
                        TokenType stack_operator = operator_stack_pop(&operator_stack);
                        Expression *rhs = expression_stack_pop(&expression_stack);
                        Expression *lhs = expression_stack_pop(&expression_stack);
                        expression_stack_push(&expression_stack, expression_binop(stack_operator, lhs, rhs));
                    } else {
                        break;
                    }
                }
                operator_stack_push(&operator_stack, token.type);
                break;
            case TOKEN_LPAREN:
                operator_stack_push(&operator_stack, TOKEN_LPAREN);
                break;
            case TOKEN_RPAREN:
                while(true) {
                    TokenType operator = operator_stack_pop(&operator_stack);
                    if (operator == TOKEN_LPAREN) break;
                    Expression* rhs = expression_stack_pop(&expression_stack);
                    Expression* lhs = expression_stack_pop(&expression_stack);
                    expression_stack_push(&expression_stack, expression_binop(operator, lhs, rhs));
                }
                break;
            case TOKEN_END_OF_FILE:
            case TOKEN_END_OF_LINE:
                while(true) {
                    if (operator_stack_is_empty(operator_stack)) {
                        if (expression_stack_is_empty(expression_stack)) {
                            return NULL;
                        }
                        Expression *expression = expression_stack_pop(&expression_stack);
                        assert(expression_stack_is_empty(expression_stack));
                        return expression;
                    }
                    TokenType operator = operator_stack_pop(&operator_stack);
                    Expression* rhs = expression_stack_pop(&expression_stack);
                    Expression* lhs = expression_stack_pop(&expression_stack);
                    expression_stack_push(&expression_stack, expression_binop(operator, lhs, rhs));
                }
                break;
            default:
                PANIC("Unkown token in parser");
                break;
        }
    }
}

long eval(Expression *expression) {
    switch (expression->type) {
        case EXPR_TYPE_MUL: return eval(expression->lhs) * eval(expression->rhs);
        case EXPR_TYPE_ADD: return eval(expression->lhs) + eval(expression->rhs);
        case EXPR_TYPE_INT: return expression->value;
        default: PANIC("Unknown expression type: %d", expression->type);
    }
}

long sum_eval(char* filename, PresedenceCompareFn presedence_cmp, AssociativityFn associativity) {
    FILE *file = fopen(filename, "r");
    Expression *expression = NULL;
    long sum = 0;
    while((expression = next_expression(file, presedence_cmp, associativity)) != NULL) {
        sum += eval(expression);
        expression_free(expression);
    }
    fclose(file);
    return sum;
}

Associativity associativity(TokenType a) {
    return ASSOCIATIVITY_LEFT;
}

int presedence_cmp_part_1(TokenType a, TokenType b) {
    return 1;
}

int presedence_cmp_part_2(TokenType a, TokenType b) {
    return a - b;
}

int main(int argc, char **argv) {
    char *filename = argv[1];
    printf("Solution 1: %ld\n", sum_eval(filename, presedence_cmp_part_1, associativity));
    printf("Solution 2: %ld\n", sum_eval(filename, presedence_cmp_part_2, associativity));
}

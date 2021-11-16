#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "sexpr.h"

#define ILLEGAL -999999999

struct s_token* global_it;

void show_token(struct s_token* head) {
    if (head == NULL) {
        printf("Invalid Expression\n");
        return;
    }

    struct s_token* it = head;
    printf("(");
    while (it != NULL) {
        printf("%s", it->val);
        it = it->next;
        if (it != NULL) {
            printf(" ");
        }
    }
    printf(")\n");
}

struct s_token* build_expr(const char* exp) {
    const char* const_op = "()+-*/:";
    int expr_len = strlen(exp);
    int expr_space_len = 0;
    char* expr_space = (char*)malloc(expr_len * 3 + 1);
    for (int i = 0; i < expr_len; i++) {
        if (strchr(const_op, exp[i]) != NULL) {
            expr_space[expr_space_len++] = ' ';
        }
        expr_space[expr_space_len++] = exp[i];
        if (strchr(const_op, exp[i]) != NULL) {
            expr_space[expr_space_len++] = ' ';
        }
    }
    expr_space[expr_space_len++] = '\0';

    char* tok = strtok(expr_space, " ");
    struct s_token* head = NULL;
    struct s_token* tail = NULL;
    while (tok != NULL) {
        struct s_token* new = create_token(tok);
        if (head == NULL) {
            head = tail = new;
        } else {
            tail->next = new;
            tail = new;
        }
        tok = strtok(NULL, " ");
    }
    free(expr_space);
    expr_space = NULL;
    return head;
}

int eval_recursive() {
    if (global_it == NULL) {
        return ILLEGAL;
    }

    int result = ILLEGAL;
    int first = 0;
    int second = 0;

    if (is_number(global_it->val) == 1) {
        result = atoi(global_it->val);
        global_it = global_it->next;
    } else if (strcmp(global_it->val, "(") == 0) {
        global_it = global_it->next;
        result = eval_recursive();
        if (global_it == NULL || strcmp(global_it->val, ")") != 0) {  // Illegal
            result = ILLEGAL;
        } else {
            global_it = global_it->next;
        }
    } else if (strcmp(global_it->val, "+") == 0) {
        global_it = global_it->next;
        first = eval_recursive();
        if (first == ILLEGAL) {
            return ILLEGAL;
        }
        second = eval_recursive();
        if (second == ILLEGAL) {
            return ILLEGAL;
        }
        result = first + second;
    } else if (strcmp(global_it->val, "-") == 0) {
        global_it = global_it->next;
        first = eval_recursive();
        if (first == ILLEGAL) {
            return ILLEGAL;
        }
        second = eval_recursive();
        if (second == ILLEGAL) {
            return ILLEGAL;
        }
        result = first - second;
    } else if (strcmp(global_it->val, "*") == 0) {
        global_it = global_it->next;
        first = eval_recursive();
        if (first == ILLEGAL) {
            return ILLEGAL;
        }
        second = eval_recursive();
        if (second == ILLEGAL) {
            return ILLEGAL;
        }
        result = first * second;
    } else if (strcmp(global_it->val, "/") == 0) {
        global_it = global_it->next;
        first = eval_recursive();
        if (first == ILLEGAL) {
            return ILLEGAL;
        }
        second = eval_recursive();
        if (second == ILLEGAL) {
            return ILLEGAL;
        }
        result = first / second;
    } else {
        result = ILLEGAL;
    }
    return result;
}

struct s_token* parse_recursive() {
    if (global_it == NULL) {
        return NULL;
    }

    struct s_token* head = NULL;
    struct s_token* tail = NULL;
    struct s_token* new = NULL;
    int new_val;
    char buf[100];

    while (global_it != NULL && strcmp(global_it->val, ")")) {
        if (strcmp(global_it->val, "(") == 0) {
            global_it = global_it->next;
            new = parse_recursive();
            if (global_it == NULL) {
                return NULL;
            }
            if (strcmp(global_it->val, ")") != 0) {  // Illegal
                return NULL;
            }
            global_it = global_it->next;
        } else if (strcmp(global_it->val, ":") == 0) {
            global_it = global_it->next;
        } else if (strcmp(global_it->val, "eval") == 0) {
            global_it = global_it->next;
            if (global_it != NULL && !strcmp(global_it->val, ":")) {
                global_it = global_it->next;
            }
            new_val = eval_recursive();
            if (new_val == ILLEGAL) {
                return NULL;
            }
            sprintf(buf, "%d", new_val);
            new = create_token(buf);
        } else {
            new = create_token(global_it->val);
            global_it = global_it->next;
        }

        if (head == NULL) {
            head = tail = new;
        } else {
            tail->next = new;
            tail = get_tail(tail);
        }
    }
    return head;
}

struct s_token* parse_expr(struct s_token* head) {
    global_it = head;
    struct s_token* result = parse_recursive();
    return result;
}

struct s_token* create_token(const char* val) {
    struct s_token* new = (struct s_token*)malloc(sizeof(struct s_token));
    new->next = NULL;
    new->val = (char*)malloc(strlen(val) + 1);  // NUL terminator
    memcpy(new->val, val, strlen(val) + 1);  // NUL terminator
    return new;
}

struct s_token* get_tail(struct s_token* cur) {
    if (cur == NULL) {
        return NULL;
    }

    struct s_token* it = cur;
    while (it->next != NULL) {
        it = it->next;
    }
    return it;
}

int is_number(const char* s) {
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        if (i == 0 && s[i] == '-') {
            continue;
        }
        if (s[i] <'0' || s[i] > '9') {
            return 0;
        }
    }
    return 1;
}

void destroy_expr(struct s_token* head) {
    struct s_token* it = head;
    while (it != NULL) {
        head = it->next;
        it->next = NULL;
        free(it->val);
        it->val = NULL;
        free(it);
        it = head;
    }
    head = NULL;
}

void evaluate_exp(const char* exp) {
    struct s_token* expr = build_expr(exp);
    struct s_token* expr_evaluated = parse_expr(expr);
    show_token(expr_evaluated);
    destroy_expr(expr);
    destroy_expr(expr_evaluated);
}

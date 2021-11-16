#ifndef SEXPR_H
#define SEXPR_H

struct s_token {
    char* val;
    struct s_token* next;
};

extern struct s_token* global_it;

struct s_token* build_expr(const char* exp);

void show_token(struct s_token* head);

struct s_token* parse_recursive();

int eval_recursive();

struct s_token* parse_expr(struct s_token* head);

struct s_token* create_token(const char* val);

struct s_token* get_tail(struct s_token* cur);

int is_number(const char* s);

void evaluate_exp(const char* exp);

void destroy_expr(struct s_token* head);

#endif  // SEXPR_H

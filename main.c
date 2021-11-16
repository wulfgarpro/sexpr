#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sexpr.h"

int main() {
    printf("=====Example Test 1=====\n");
    char* exp1 = "(foo(eval:(*2 (+1 1))))";
    printf("%s\n", exp1);
    evaluate_exp(exp1);
    printf("\n");

    printf("=====Example Test 2=====\n");
    char* exp2 = "(foo (:eval(* 2 (+ 1 1))) (:eval(* 2 (+ 1 1))))";
    printf("%s\n", exp2);
    evaluate_exp(exp2);
    printf("\n");

    printf("=====Example Test 3=====\n");
    char* exp3 = "(foo (:eval (* 22 (+ 1 16))))";
    printf("%s\n", exp3);
    evaluate_exp(exp3);
    printf("\n");

    printf("=====User Test=====\n");
    char exp[1000];
    (void)fgets(exp, sizeof(exp), stdin);
    int len = strlen(exp);
    if (len > 0 && exp[len - 1] == '\n') {
        exp[len - 1] = '\0';
    }
    evaluate_exp(exp);

    return 0;
}

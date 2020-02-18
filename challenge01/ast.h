#ifndef AST_H
#define AST_H

#include "svec.h"

typedef struct ast
{
    //Command string
    char *cmnd;

    struct ast *larg;
    struct ast *rarg;

    //String array of arguments
    svec *args;
} ast;

ast *make_ast_value(svec *args);
ast *make_ast_op(char op, ast *a0, ast *a1);
void free_ast(ast *ast);
int ast_eval(ast *ast);
void print_ast(ast *ast);

#endif
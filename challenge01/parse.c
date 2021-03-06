#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "ast.h"
#include "parse.h"

int find_first_index(svec *toks, const char *tt)
{
    for (size_t ii = 0; ii < toks->size; ii++)
    {
        if (!strcmp(toks->data[ii], tt))
        {
            return ii;
        }
        ii++;
    }

    return -1;
}

int contains(svec *toks, const char *tt)
{
    return find_first_index(toks, tt) != -1;
}

svec *
slice(svec *xs, int i0, int i1)
{
    svec *ys = make_svec();
    for (int ii = i0; ii < i1; ++ii)
    {
        svec_push_back(ys, xs->data[ii]);
    }
    return ys;
}

shell_ast *
parse(svec *toks)
{
    const char *ops[] = {";", "||", "&&", "&", "|", "<", ">"};

    for (int ii = 0; ii < 7; ++ii)
    {
        const char *op = ops[ii];

        if (contains(toks, op))
        {
            int jj = find_first_index(toks, op);
            svec *xs = slice(toks, 0, jj);
            svec *ys = slice(toks, jj + 1, toks->size);
            shell_ast *ast = make_ast_op(op, parse(xs), parse(ys));
            free(xs);
            free(ys);
            return ast;
        }
    }

    shell_ast *ast = make_ast_value(slice(toks,0,toks->size));
    return ast;
}
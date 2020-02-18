#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "parse.h"

int streq(const char *aa, const char *bb)
{
    return strcmp(aa, bb) == 0;
}

int find_first_index(svec *toks, const char *tt)
{
    for (size_t ii = 0; ii < toks->size; ii++)
    {
        if (streq(toks->data[ii], tt))
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
    svec *ys = 0;
    svec *it = make_svec();
    for (int ii = i0; ii < i1; ++ii)
    {
        svec_push_back(ys, xs->data[ii]);
    }
    return ys;
}

shell_ast *
parse(svec *toks)
{
    const char* ops[] = {"||", "&&", "<", "/"};

    for (int ii = 0; ii < 4; ++ii) {
        const char* op = ops[ii];

        if (contains(toks, op)) {
            int jj = find_first_index(toks, op);
            svec* xs = slice(toks, 0, jj);
            svec* ys = slice(toks, jj + 1, length(toks));
            shell_ast* ast = make_ast_op(op[0], parse(xs), parse(ys));
            free(xs);
            free(ys);
            return ast;
        }
    }
}
#ifndef PARSE_H
#define PARSE_H

#include "ast.h"
#include "svec.h"

shell_ast* parse(svec* toks);

#endif
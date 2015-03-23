/**

@file condparser.h

========================================

Grammar:

    conds       := expr cond

    cond        := '=' expr cond_tail

    cond_tail   := cond
                 | epsilon

    expr        := term expr_tail

    expr_tail   := '+' expr
                 | '-' expr
                 | epsilon

    term        := atom term_tail

    term_tail   := '*' term
                 | '/' term
                 | epsilon

    atom        := '-' atom_tail
                 | atom_tail

    atom_tail   := id
                 | number
                 | '(' expr ')'

    id          := [a-zA-Z]+ [0-9]+

    number      :=  [+-] [0-9]+ (\.[0-9]+)? ([eE][-+]?[0-9]+)?

========================================

*/

#ifndef _GENERAL_LINEAR_LEAST_SQUARES_CONDPARSER_H_
#define _GENERAL_LINEAR_LEAST_SQUARES_CONDPARSER_H_

#endif //_GENERAL_LINEAR_LEAST_SQUARES_CONDPARSER_H_

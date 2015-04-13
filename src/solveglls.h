/**
    @file glls.h
*/

#ifndef GENERAL_LINEAR_LEAST_SQUARES_SOLVE_GLLS_H
#define GENERAL_LINEAR_LEAST_SQUARES_SOLVE_GLLS_H

#include <vector>
#include <list>
#include <utility>

struct GllsProblem
{
    int xSize;
    std::vector<std::pair<int, double> > reservedX;
    std::vector<double> coef;
};

void arrangeX(
        GllsProblem &,
        const std::vector<std::pair<int, double> > &xs
);

void arrangeY(
        GllsProblem &,
        const std::list<std::vector<std::pair<int, double> > > &ys
);

/**
    @return a full length `x` vector
*/
std::vector<double> solve(const GllsProblem &);

#endif


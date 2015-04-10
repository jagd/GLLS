/**
    @file glls.h
*/

#ifndef GENERAL_LINEAR_LEAST_SQUARES_GLLS_H
#define GENERAL_LINEAR_LEAST_SQUARES_GLLS_H

#include <vector>
#include <list>

struct GllsProblem
{
    int xSize;
    std::vector<double> reservedX;
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

#endif //GENERAL_LINEAR_LEAST_SQUARES_GLLS_H


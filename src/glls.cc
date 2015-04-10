#include "glls.h"

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

void arrangeX(
        GllsProblem &g,
        const std::vector<std::pair<int, double> > &rxs
)
{
    assert(g.coef.size() % (g.xSize+1) == 0);
    assert(g.reservedX.size() == 0);
    const int origCols = g.xSize + 1;
    const int cols = origCols - rxs.size();
    const int rows = g.coef.size()/origCols;
    std::vector<double> coef(rows*cols);
    g.reservedX.resize(rxs.size());
    auto xs = rxs;
    std::sort(xs.begin(), xs.end(),
            [](
                    const std::pair<int, double> &a,
                    const std::pair<int, double> &b
            ) {
                return a.second < b.second;
            });
    for (int row = 0; row < rows; ++row) {
        coef[cols*(row+1) - 1] = g.coef[origCols*(row+1) - 1];
    }
    int origCol = 0;
    for (int i = 0; i < xs.size(); ++i) {
        const auto &x = xs[i];
        for (; origCol<x.first; ++origCol) {
            for (int row = 0; row < rows; ++row) {
                coef[cols*row + origCol - i] = g.coef[origCols*row + origCol];
            }
        }
        for (int row = 0; row < rows; ++row) {
            coef[cols*(row+1) - 1] += g.coef[origCols*row + origCol]*x.second;
        }
        g.reservedX[i] = x.first;
        ++origCol;
    }
    g.coef = std::move(coef);
    g.xSize -= g.reservedX.size();
}

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
                return a.first < b.first;
            });
    for (int row = 0; row < rows; ++row) {
        coef[cols*(row+1) - 1] = g.coef[origCols*(row+1) - 1];
    }
    int origCol = 0;
    for (std::size_t i = 0; i < xs.size(); ++i) {
        const auto &x = xs[i];
        assert(x.first >= origCol);
        assert(x.first < g.xSize);
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
    for (; origCol<g.xSize; ++origCol) {
        for (int row = 0; row < rows; ++row) {
            coef[cols*row + origCol - xs.size()]
                    = g.coef[origCols*row + origCol];
        }
    }
    g.coef = std::move(coef);
    g.xSize -= g.reservedX.size();
}

void arrangeY(
        GllsProblem &g,
        const std::list<std::vector<std::pair<int, double> > > &ys
)
{
    assert(g.coef.size() % (g.xSize+1) == 0);
    assert(ys.size() > 0);
    const int cols = g.xSize + 1;
    const int rows = ys.size();
    std::vector<double> coef;
    coef.reserve(rows*cols);
    for (const auto &eq : ys) {
        std::vector<double> c(cols);
        for (const auto &y : eq) {
            for (int i = 0; i < cols; ++i) {
                c[i] += y.second * g.coef[y.first*cols + i];
            }
        }
        std::copy(c.cbegin(), c.cend(), std::back_inserter(coef));
    }
    g.coef = coef;
}

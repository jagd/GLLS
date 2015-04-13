#include "solveglls.h"
#include "condparser.h"

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>

void arrangeX(
        GllsProblem &g,
        const std::vector<std::pair<int, double> > &rxs
)
{
    assert(g.xSize > 0);
    assert(g.coef.size() % (g.xSize+1) == 0);
    assert(g.reservedX.size() == 0);
    const int origCols = g.xSize + 1;
    const int cols = origCols - rxs.size();
    const int rows = g.coef.size()/origCols;
    std::vector<double> coef(rows*cols);
    g.reservedX.resize(rxs.size());
    auto xs = rxs;
    std::sort(xs.begin(), xs.end(),
        [](const std::pair<int, double> &a, const std::pair<int, double> &b)
        { return a.first < b.first; });
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
        ++origCol;
    }
    for (; origCol<g.xSize; ++origCol) {
        for (int row = 0; row < rows; ++row) {
            coef[cols*row + origCol - xs.size()]
                    = g.coef[origCols*row + origCol];
        }
    }
    g.coef = std::move(coef);
    g.reservedX = xs;
    g.xSize -= g.reservedX.size();
}

void arrangeY(
        GllsProblem &g,
        const std::list<std::vector<std::pair<int, double> > > &ys
)
{
    assert(g.xSize > 0);
    assert(g.coef.size() % (g.xSize+1) == 0);
    assert(ys.size() > 0);
    const int cols = g.xSize + 1;
    const int rows = ys.size();
    std::vector<double> coef;
    coef.reserve(rows*cols);
    for (const auto &eq : ys) {
        std::vector<double> c(cols);
        for (const auto &y : eq) {
            if (y.first == CondDict::ID_CONST) {
                c[cols-1] += y.second;
            } else {
                assert(y.first >= 0);
                for (int i = 0; i < cols; ++i) {
                    c[i] += y.second * g.coef[y.first*cols + i];
                }
            }
        }
        std::copy(c.cbegin(), c.cend(), std::back_inserter(coef));
    }
    g.coef = coef;
}

boost::numeric::ublas::vector<double>
solveLeastSquare(
        const boost::numeric::ublas::matrix<double> &m,
        const boost::numeric::ublas::vector<double> &b
    )
{
    using namespace boost::numeric::ublas;
    permutation_matrix<std::size_t> pm(m.size1());
    vector<double> x;
    matrix<double> a;
    axpy_prod(b, m, x, true);
    axpy_prod(trans(m), m, a, true);
    lu_factorize(a, pm);
    lu_substitute(a, pm, x);
    return x;
}

boost::numeric::ublas::vector<double>
solveMinNorm(
        const boost::numeric::ublas::matrix<double> &m,
        const boost::numeric::ublas::vector<double> &b
)
{
}

boost::numeric::ublas::vector<double>
solveExact(
        const boost::numeric::ublas::matrix<double> &m,
        const boost::numeric::ublas::vector<double> &b
)
{
    using namespace boost::numeric::ublas;
    auto a = m;
    auto x = b;
    permutation_matrix<std::size_t> pm(a.size1());
    lu_factorize(a, pm);
    lu_substitute(a, pm, x);
    return x;
}

template<class IT> std::vector<double>
fullX(IT it, GllsProblem const &g)
{
    std::vector<double> v;
    v.reserve(g.xSize+g.reservedX.size());
    int i = 0;
    for (const auto &s : g.reservedX) {
        for (; i < s.first; ++i) {
            v.push_back(*(it++));
        }
        v.push_back(s.second);
        ++i;
    }
    for (; i < static_cast<int>(g.xSize+g.reservedX.size()); ++i) {
        v.push_back(*(it++));
    }
    return v;
}

std::vector<double> solve(GllsProblem const &g)
{
    using namespace boost::numeric::ublas;

    assert(g.xSize > 0);
    assert(g.coef.size() % (g.xSize + 1) == 0);
    const int cols = g.xSize + 1;
    const int rows = g.coef.size() / (g.xSize + 1);
    matrix<double> m(rows, g.xSize);
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < g.xSize; ++col) {
            m(row, col) = g.coef[row*cols + col];
        }
    }
    vector<double> b(rows);
    for (int row = 0; row < rows; ++row) {
            b(row) = -g.coef[(row+1)*cols - 1];
    }
    vector<double> x;
    if (rows > g.xSize) {
        x = solveLeastSquare(m, b);
    } else if (rows < g.xSize) {
        x = solveMinNorm(m, b);
    } else {
        x = solveExact(m, b);
    }
    return fullX(x.begin(), g);
}


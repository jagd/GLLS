#include "glls.h"
#include <vector>
#include <cassert>

void arrangeX(
        GllsProblem &g,
        const std::vector<std::pair<int, double> > &xs
)
{
    assert(g.coef.size() % (g.xSize+1) == 0);
}

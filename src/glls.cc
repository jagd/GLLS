#include "glls.h"

std::vector<double> glls(std::istream &s)
{
    GllsParser gp(s, true);
    auto g = gp.run();
    arrangeX(g, gp.xValues());
    arrangeY(g, gp.yConds());
    return solve(g);
}

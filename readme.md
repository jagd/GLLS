#   Introduction
This is a [least squares][] solver for general linear systems.  Inspired by 
the solution of magnetic fields in rotationally symmetric gyrotrons.

#   Input
The input is written in ascii text, e.g.:

    # the name of the unknown vector (only consist of letters)
    I
    # arbitrary number of name for the Y components of the linear system
    # only consist of letters
    Bz Br Psi
    # the numerical coefficient matrix M 
    # Bz0  = M00*I0 + M01*I1 + ...
    # Br0  = M10*I0 + M11*I1 + ...
    # Psi0 = M20*I0 + M21*I1 + ...
    # Bz1  = M30*I0 + M31*I1 + ...
    # ....
    1   2   3
    2   3.3 4
    5   6   7
    4   4   3e8
    constraints
    # each line contains an equation, can include several ='s
    Br0 = 0 = Br1
    Bz1 + 5 = Bz0 * 2 + (2 + 4)*Bz3 = (Psi3 + Bz1*2) * 6 = (Bz0 + Bz2)
    2*I1 + 1 = (1+2) * 3

#   Output
After solve the equation of `[M][I] = [B]`, the unknown vector will be given,
in the  above  case the vector `I`.

[least squares]: http://en.wikipedia.org/wiki/Least_squares

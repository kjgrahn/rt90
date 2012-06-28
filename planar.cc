/* planar.cc
 *
 * Copyright (c) 2012 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "planar.h"

#include <iostream>

std::ostream& Planar::put(std::ostream& os) const
{
    return os << x << ' ' << y;
}


/**
 * The square of the distance a--b.
 */
double distance2(const Planar& a, const Planar& b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx*dx + dy*dy;
}

/* planar.cc
 *
 * Copyright (c) 2012 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "planar.h"

/**
 * The square of the distance a--b.
 */
double distance2(const Planar& a, const Planar& b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx*dx + dy*dy;
}

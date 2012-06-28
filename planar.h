/* -*- c++ -*-
 *
 * Copyright (c) 2012 Jörgen Grahn
 * All rights reserved.
 */
#ifndef RT90_PLANAR_H
#define RT90_PLANAR_H

#include <iosfwd>

/**
 * A planar coordinate in the numerical range favored by RT90 and
 * SWEREF 99. Based on double, which happens to be PROJ.4's favorite
 * type too.
 */
class Planar {
public:
    Planar() : x(0), y(0) {}
    Planar(double x, double y) : x(x), y(y) {}
    std::ostream& put(std::ostream& os) const;
    double x;
    double y;
};

double distance2(const Planar& a, const Planar& b);

inline
std::ostream& operator<< (std::ostream& os, const Planar& val) {
    return val.put(os);
}

#endif

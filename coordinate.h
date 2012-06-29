/* -*- c++ -*-
 *
 * Copyright (c) 2012 Jörgen Grahn
 * All rights reserved.
 */
#ifndef RT90_COORDINATE_H
#define RT90_COORDINATE_H

#include <iosfwd>

/**
 * A planar coordinate in RT90. Unlike Planar, suited for this
 * particular application rather than libproj's interface:
 * - 1 m resolution
 * - an explicit 'invalid' value
 * - sanity checks
 * - conversion to/from various string forms
 */
class Rt90 {
public:
    Rt90() : north(0), east(0) {}
    Rt90(unsigned north, unsigned east) : north(north), east(east) {}
    std::ostream& put(std::ostream& os) const;
    double x;
    double y;
};


class Sweref99;


inline
std::ostream& operator<< (std::ostream& os, const Rt90& val) {
    return val.put(os);
}

inline
std::ostream& operator<< (std::ostream& os, const Sweref99& val) {
    return val.put(os);
}

#endif

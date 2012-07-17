/* -*- c++ -*-
 *
 * Copyright (c) 2012 Jörgen Grahn
 * All rights reserved.
 */
#ifndef RT90_COORDINATE_H
#define RT90_COORDINATE_H

#include "direction.h"

#include <iosfwd>
#include <set>


/**
 * RT90 accuracy for both input and output, i.e.  whether they
 * should be output in 4--7 digits (1km--1m), and which number of
 * digits should be tolerated as input.
 */
class Accuracy {
public:
    Accuracy& operator<< (unsigned n);
    bool empty() const { return input.empty(); }

    bool tolerated(unsigned n) const;
    unsigned format(unsigned n) const;

private:
    unsigned output;
    std::set<unsigned> input;

    bool contains(unsigned n) const;
};


/**
 * A planar coordinate in RT90. Unlike Planar, suited for this
 * particular application rather than libproj's interface:
 * - 1 m resolution (accuracy not remembered)
 * - an explicit 'invalid' value
 * - sanity checks
 * - conversion to/from various string forms
 */
class Rt90 {
public:
    Rt90() : north(0), east(0) {}
    Rt90(unsigned north, unsigned east);
    bool operator== (const Rt90& other) const;
    bool valid() const { return north; }
    std::ostream& put(std::ostream& os) const;
    std::ostream& put(std::ostream& os, const Accuracy& accuracy) const;

    unsigned north;
    unsigned east;
};


/**
 * A planar coordinate in SWEREF 99, similar to class Rt90.
 */
class Sweref99 {
public:
    Sweref99() : north(0), east(0) {}
    Sweref99(unsigned north, unsigned east);
    bool operator== (const Sweref99& other) const;
    bool valid() const { return north; }
    std::ostream& put(std::ostream& os) const;

    unsigned north;
    unsigned east;
};


class Transform;
Sweref99 convert(const Transform& t, const Rt90& val);
Rt90 convert(const Transform& t, const Sweref99& val);


bool parse(const Accuracy& accuracy, bool rt90,
	   const char* s,
	   unsigned& north, unsigned& east);
bool parse(const Accuracy& accuracy, bool rt90,
	   const std::string& s,
	   unsigned& north, unsigned& east);
bool parse(const Accuracy& accuracy,
	   const char* a, const char* b,
	   unsigned& north, unsigned& east);

inline
std::ostream& operator<< (std::ostream& os, const Rt90& val) {
    return val.put(os);
}

inline
std::ostream& operator<< (std::ostream& os, const Sweref99& val) {
    return val.put(os);
}

#endif

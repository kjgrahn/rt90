/* coordinate.cc
 *
 * Copyright (c) 2012 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "coordinate.h"

#include "planar.h"
#include "transform.h"

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cctype>


namespace {

    /**
     * True iff a < b < c.
     */
    bool in_range(unsigned a, unsigned b, unsigned c)
    {
	return a < b && b < c;
    }

    /**
     * The number of digits in 'n'. We're only interested
     * in the range 4--7.
     */
    unsigned digits_of(unsigned n)
    {
	unsigned digits = 0;
	if(n>999) digits = 4;
	if(n>9999) digits = 5;
	if(n>99999) digits = 6;
	if(n>999999) digits = 7;
	if(n>9999999) digits = 8;
	return digits;
    }
}


/**
 * Construct from north and east coordinates, which may
 * be shortened forms (like 1km resolution).
 */
Rt90::Rt90(const unsigned n, const unsigned e)
    : north(n),
      east(e)
{
    while(north < 1000000) {
	north *= 10;
	east *= 10;
    }

    if(!in_range(6100000, north, 7800000) ||
       !in_range(1200000, east,  1900000)) {
	*this = Rt90();
    }
}


/**
 * Construct from north and east coordinates. Unlike
 * Rt90, these can only be in 1m resolution.
 */
Sweref99::Sweref99(const unsigned n, const unsigned e)
    : north(n),
      east(e)
{
    if(!in_range(6100000, north, 7800000) ||
       !in_range(258000, east,  926000)) {
	*this = Sweref99();
    }
}


bool Rt90::operator== (const Rt90& other) const
{
    return north==other.north &&
	east==other.east;
}


bool Sweref99::operator== (const Sweref99& other) const
{
    return north==other.north &&
	east==other.east;
}


std::ostream& Rt90::put(std::ostream& os) const
{
    return os << north << ' ' << east;
}


std::ostream& Rt90::put(std::ostream& os, const Accuracy& accuracy) const
{
    return os << accuracy.format(north)
	      << ' '
	      << accuracy.format(east);
}


std::ostream& Sweref99::put(std::ostream& os) const
{
    return os << north << ' ' << east;
}


Accuracy& Accuracy::operator<< (unsigned n)
{
    assert(in_range(3, n, 8));
    output = n;
    input.insert(n);
    return *this;
}


/**
 * True iff 'n' has a tolerable number of digits.
 */
bool Accuracy::tolerated(unsigned n) const
{
    return contains(digits_of(n));
}


/**
 * 'n' rounded to the correct number of digits.
 */
unsigned Accuracy::format(unsigned n) const
{
    switch(output) {
    case 7:
	return n;
    case 6:
	return (n+5)/10;
    case 5:
	return (n+50)/100;
    case 4:
	return (n+500)/1000;
    }
    assert(0);
}


bool Accuracy::contains(unsigned n) const
{
    return input.find(n) != input.end();
}


namespace {

    template <class T>
    Planar planar(T val)
    {
	return Planar(val.north, val.east);
    }

    template <class T>
    T unplanar(const Planar& val)
    {
	return T(val.x + .5, val.y + .5);
    }
}


/**
 * Conversion RT90 --> SWEREF99, using 't'.
 */
Sweref99 convert(const Transform& t, const Rt90& val)
{
    const Planar p = planar(val);
    const Planar q = t.forward(p);
    return unplanar<Sweref99>(q);
}


/**
 * Conversion SWEREF99 --> RT90, using 't'.
 */
Rt90 convert(const Transform& t, const Sweref99& val)
{
    const Planar p = planar(val);
    const Planar q = t.backward(p);
    return unplanar<Rt90>(q);
}


/**
 * Parse a string which contains only whitespace and two unsigned
 * decimal numbers. Return true iff these can conceivably be a RT90 or
 * SWEREF99 coordinate, i.e. consist of the right number of digits.
 *
 * Sanity checks on the actual numbers aren't done here; that's the
 * job of classes Rt90/Sweref99.
 */
bool parse(const Accuracy& accuracy,
	   const char* s,
	   unsigned& north, unsigned& east)
{
    /* It would have been nice if strtoul(3) had been enough.
     * Unfortunately it's a bit too tolerant; you'd get negative
     * numbers and large unsigned longs squeezed into an unsigned.
     * So we base this on old-fashioned manual parsing.
     *
     * "   645102 136732   "
     *     a      b
     */
    while(std::isspace(*s)) s++;
    const char* const a = s;
    if(!*a || !std::isdigit(*a) || *a=='0') return false;
    while(std::isdigit(*s)) s++;
    if(s-a > 7) return false;

    while(std::isspace(*s)) s++;
    const char* const b = s;
    if(!*b || !std::isdigit(*b) || *b=='0') return false;
    while(std::isdigit(*s)) s++;
    if(s-b > 7) return false;

    while(std::isspace(*s)) s++;
    if(*s) return false;

    /* Nothing much can go wrong after this point, so we can skip
     * strtoul(3) error checks.
     */
    north = std::strtoul(a, 0, 10);
    east = std::strtoul(b, 0, 10);

    if(digits_of(north)==7 && digits_of(east)==6) return true;

    return digits_of(north)==digits_of(east) &&
	accuracy.tolerated(north);
}


/**
 * As parse(... const char* ...).
 */
bool parse(const Accuracy& accuracy,
	   const std::string& s,
	   unsigned& north, unsigned& east)
{
    return parse(accuracy, s.c_str(), north, east);
}


/**
 * As the one-string parse(), with "$a $b" as the string.
 *
 * We're also more sloppy with the format checking, since this is for
 * parsing numbers given explicitly on the command line, not random
 * data in some file.
 */
bool parse(const Accuracy& accuracy,
	   const char* a, const char* b, 
	   unsigned& north, unsigned& east)
{
    char* end;
    north = std::strtoul(a, &end, 10);
    if(*end) north = 0;
    east = std::strtoul(b, &end, 10);
    if(*end) east = 0;

    if(digits_of(north)==7 && digits_of(east)==6) return true;

    return digits_of(north)==digits_of(east) &&
	accuracy.tolerated(north);
}

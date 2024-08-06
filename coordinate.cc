/* coordinate.cc
 *
 * Copyright (c) 2012 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "coordinate.h"

#include <iostream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cctype>


namespace {

    /**
     * True iff b is in [a..c).
     */
    bool in_range(unsigned a, unsigned b, unsigned c)
    {
	return a <= b && b < c;
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


/**
 * Parse a string which contains only whitespace and two unsigned
 * decimal numbers. Return true iff these can conceivably be a RT90 or
 * SWEREF99 coordinate, i.e. consist of the right number of digits.
 *
 * You explicitly (through 'rt90') ask for either kind of coordinate.
 *
 * Sanity checks on the actual numbers aren't done here; that's the
 * job of classes Rt90/Sweref99.
 */
bool parse(const Accuracy& accuracy,
	   bool rt90,
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

    if(digits_of(north)==7 && digits_of(east)==6) return !rt90;

    return digits_of(north)==digits_of(east) && rt90 &&
	accuracy.tolerated(north);
}


/**
 * As parse(... const char* ...).
 */
bool parse(const Accuracy& accuracy,
	   bool rt90,
	   const std::string& s,
	   unsigned& north, unsigned& east)
{
    return parse(accuracy, rt90, s.c_str(), north, east);
}


/**
 * As the one-string parse(), with "$a $b" as the string.
 *
 * We're more sloppy with the format checking, since this is for
 * parsing numbers given explicitly on the command line, not random
 * data in some file.
 *
 * Also, this one swallows both RT90 and SWEREF 99.
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


namespace {

    bool isdigit(char ch)
    {
	return std::isdigit(static_cast<unsigned char>(ch));
    }

    std::vector<const char*> find_numbers(const char* p)
    {
	std::vector<const char*> acc;

	char prev = 'x';
	while (*p) {
	    if (isdigit(*p) && !isdigit(prev)) {
		acc.push_back(p);
	    }
	    prev = *p++;
	}
	return acc;
    }
}

/**
 * Parse a string containing exactly two numbers, and other crud. This
 * is for the rt90 --tolerant option.
 *
 * Unfortunately, since we want to fix swapped coordinates (east,
 * north) we need form four coordinates, and swap the numbers if that
 * helps. Then the calling code then forms coordinates again --
 * stupid, but I don't want to do major refactoring and incolve
 * accurracy, whether the user asked for RT90 or not, and so on.
 */
bool parse(const std::string& s,
	   unsigned& north, unsigned& east)
{
    const char* const ss = s.c_str();
    const auto v = find_numbers(ss);
    if (v.size()!=2) return false;
    north = std::strtoul(v[0], nullptr, 10);
    east  = std::strtoul(v[1], nullptr, 10);

    Rt90 rb {east, north};
    Sweref99 sb {east, north};

    if (rb.valid() || sb.valid()) std::swap(north, east);
    return true;
}

/* coordinate.cc
 *
 * Copyright (c) 2012 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "coordinate.h"

#include <cassert>

namespace {

    /**
     * True iff a < b < c.
     */
    bool in_range(unsigned a, unsigned b, unsigned c)
    {
	return a < b && b < c;
    }
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
    unsigned digits = 0;
    if(n>999) digits = 4;
    if(n>9999) digits = 5;
    if(n>99999) digits = 6;
    if(n>999999) digits = 7;
    if(n>9999999) digits = 8;
    return contains(digits);
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
}


bool Accuracy::contains(unsigned n) const
{
    return input.find(n) != input.end();
}

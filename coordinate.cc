/* coordinate.cc
 *
 * Copyright (c) 2012 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "coordinate.h"

Accuracy& Accuracy::operator<< (unsigned n)
{
    output = n;
    input.insert(n);
    return *this;
}

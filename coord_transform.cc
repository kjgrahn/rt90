/*
 * Copyright (c) 2012, 2013 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "coordinate.h"

#include "planar.h"
#include "transform.h"


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

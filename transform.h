/* -*- c++ -*-
 *
 * Copyright (c) 2012 Jörgen Grahn
 * All rights reserved.
 */
#ifndef RT90_TRANSFORM_H
#define RT90_TRANSFORM_H

#include "planar.h"


/**
 * Conversion RT90 2.5 gon V --> SWEREF 99 TM.
 */
class Forward {
public:
    Forward();
    Planar operator() (const Planar& p) const;
private:
    Forward(const Forward&);
    Forward& operator= (const Forward&);

    void *pj;
};


/**
 * Conversion SWEREF 99 TM --> RT90 2.5 gon V.
 */
class Backward {
public:
    Planar operator() (const Planar& p) const { return p; }
};

#endif

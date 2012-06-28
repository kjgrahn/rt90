/* -*- c++ -*-
 *
 * Copyright (c) 2012 Jörgen Grahn
 * All rights reserved.
 */
#ifndef RT90_TRANSFORM_H
#define RT90_TRANSFORM_H

#include "planar.h"


/**
 * Conversion RT90 2.5 gon V --> SWEREF 99 TM ("forward")
 * and the reverse direction.
 */
class Transform {
public:
    Transform();
    ~Transform();

    Planar forward(const Planar& p) const;
    Planar backward(const Planar& p) const;

private:
    Transform(const Transform&);
    Transform& operator= (const Transform&);

    void* a;
    void* b;
};

#endif

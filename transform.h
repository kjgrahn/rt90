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

    static const char* pj_release();

private:
    Transform(const Transform&);
    Transform& operator= (const Transform&);

    /* 'a' and 'b' are really of type projPJ, but that's a typedef for
     * void*, and I doubt libproj can change that in future revisions
     * without breaking code.
     *
     * I don't want to poison all my code with proj_api.h's bizarre
     * contents, and they didn't choose a system where a forward-
     * declaration would have sufficed ... so I just use void*.
     */
    void* a;
    void* b;
};

#endif

/* -*- c++ -*-
 *
 * Copyright (c) 2012 Jörgen Grahn
 * All rights reserved.
 */
#ifndef RT90_TRANSFORM_H
#define RT90_TRANSFORM_H

#include "planar.h"

#include <string>

struct PJconsts;

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

    static std::string pj_release();
    static void morons();

private:
    Transform(const Transform&);
    Transform& operator= (const Transform&);

    /* The official post-PROJ.4 API has PJ* rather than PJConsts*, but
     * I don't see how I can use the PJ typedef can be forward-
     * declared. So I use the internal type name and blame them when
     * that breaks. They had their chance to fix this.
     */
    PJconsts* const pj;
};

#endif

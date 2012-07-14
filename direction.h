/* -*- c++ -*-
 *
 * Copyright (c) 2012 Jörgen Grahn
 * All rights reserved.
 */
#ifndef RT90_DIRECTION_H
#define RT90_DIRECTION_H


/**
 * Allowed transformation directions:
 * - RT90 -> SWEREF 99
 * - SWEREF 99 -> RT90
 * - either one
 */
enum Direction {
    BOTH_DIRECTIONS,
    FROM_RT90,
    TO_RT90
};

#endif

/* -*- c++ -*-
 *
 * Copyright (c) 2012 J�rgen Grahn
 * All rights reserved.
 */
#ifndef RT90_LMV_CTRL_H
#define RT90_LMV_CTRL_H

#include "planar.h"

namespace lmv {

    /**
     * The four control points published by Lantm�teriet.
     *
     * Samband och parametrar
     * Lantm�teriet I-Divisionen Geodesi
     * 2006-05-03
     * <http://www.lantmateriet.se/upload/filer/kartor/
     * geodesi_gps_och_detaljmatning/Transformationer/
     * SWEREF99_RT90_Samband/kontrollpunkter.pdf>
     *
     */
    struct Lmv {
	struct {
	    Planar A;
	    Planar B;
	    Planar C;
	    Planar D;
	} rt90;
	struct {
	    Planar A;
	    Planar B;
	    Planar C;
	    Planar D;
	} sweref99;
    };

    extern const Lmv lmv;
}

#endif

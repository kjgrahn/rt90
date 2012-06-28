/* transform.cc
 *
 * Copyright (c) 2012 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "transform.h"

#include <proj_api.h>

namespace {

    projUV cvt(const Planar& p)
    {
	projUV uv;
	uv.u = p.x;
	uv.v = p.y;
	return uv;
    }

    Planar cvt(const projUV& uv)
    {
	Planar p;
	p.x = uv.u;
	p.y = uv.v;
	return p;
    }
}

Forward::Forward()
{
    static char* argv[] = {"ellps=WGS84",
			   "proj=tmerc",
			   "lat_0=0",
			   "lon_0=15.80628452944445",
			   "k=1.00000561024",
			   "x_0=1500064.274",
			   "y_0=-667.711"};

    pj = pj_init(sizeof argv / sizeof *argv, argv);
}

Planar Forward::operator() (const Planar& p) const
{
    Planar q = cvt(pj_fwd(cvt(p), pj));
    return q;
}

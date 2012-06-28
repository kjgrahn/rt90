/* transform.cc
 *
 * Copyright (c) 2012 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "transform.h"

#include <proj_api.h>
#include <cassert>


Transform::Transform()
    : a(pj_init_plus("+ellps=WGS84 "
		     "+proj=tmerc "
		     "+lat_0=0 "
		     "+lon_0=15.80628452944445 "
		     "+k=1.00000561024 "
		     "+x_0=1500064.274 "
		     "+y_0=-667.711")),
      b(pj_init_plus("+init=epsg:3006"))
{
    assert(a);
    assert(b);
}


Transform::~Transform()
{
    pj_free(a);
    pj_free(b);
}


namespace {

    /**
     * Helper function, to cope with the weird pj_transform()
     * interface.
     */
    Planar transform(projPJ src, projPJ dst, Planar coord)
    {
	double x = coord.x;
	double y = coord.y;
	double z = 0;
	int rc = pj_transform(src, dst,
			      1, 1,
			      &y, &x, &z);
	assert(!rc);
	return Planar(x, y);
    }
}


Planar Transform::forward(const Planar& p) const
{
    return transform(a, b, p);
}


Planar Transform::backward(const Planar& p) const
{
    return transform(b, a, p);
}

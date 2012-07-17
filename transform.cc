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
      b(pj_init_plus("+proj=utm "
		     "+zone=33 "
		     "+ellps=GRS80 "
		     "+towgs84=0,0,0,0,0,0,0 "
		     "+units=m "
		     "+no_defs"))
{
    /* Projection 'a' is from RT 90 2.5 gon V to SWEREF 99.  The
     * incantation is taken from
     * http://wiki.openstreetmap.org/wiki/Converting_to_WGS84#Sweden
     * but the exact numbers come from
     *
     * EPSG=3847
     * SWEREF99 / RT90 2.5 gon V emulation
     *
     * Unclear if the wiki was sloppy with the fine details, or if
     * it's an effect of a difference between WGS84 and SWEREF99.
     *
     * Projection 'b' is translation SWEREF99 lat/long -> planar
     * coordinates. And for that it seems EPSG=3006 from the libproj
     * distribution is correct.
     *
     * EPSG=3006
     * SWEREF99 TM
     *
     */
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
	/* Unclear why x and y are reversed here. Probably for the
	 * same reason you invoke cs2cs(1) with '-rs' when you use the
	 * commandline.  I also cannot explain the zero 'z'
	 * coordinate.
	 *
	 * Anyway, the point is: doing it this way makes the tests
	 * pass.
	 */
	assert(!rc);
	return Planar(x, y);
    }
}


/**
 * Conversion RT90 2.5 gon V --> SWEREF 99 TM, i.e. "forward".
 */
Planar Transform::forward(const Planar& p) const
{
    return transform(a, b, p);
}


/**
 * Conversion SWEREF 99 TM ---> RT90 2.5 gon V.
 */
Planar Transform::backward(const Planar& p) const
{
    return transform(b, a, p);
}


const char* Transform::pj_release()
{
    return pj_get_release();
}

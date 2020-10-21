/* transform.cc
 *
 * Copyright (c) 2012 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "transform.h"

#include <proj.h>
#include <cassert>

void Transform::morons()
{
    proj_context_use_proj4_init_rules(nullptr, true);
}


Transform::Transform()
    : pj(proj_create_crs_to_crs(nullptr,
				"+init=epsg:3021",
				"+init=epsg:3006",
				nullptr))
{
    assert(pj);
}


Transform::~Transform()
{
    proj_destroy(pj);
}


namespace {

    PJ_COORD convert(Planar coord)
    {
	return proj_coord(coord.y, coord.x, 0, 0);
    }

    Planar convert(PJ_COORD coord)
    {
	return {coord.xy.y, coord.xy.x};
    }

    /**
     * Helper function, to cope with the weird pj_transform()
     * interface.
     */
    template <PJ_DIRECTION dir>
    Planar transform(PJ* pj, Planar coord)
    {
	auto c = proj_trans(pj, dir, convert(coord));
	return convert(c);
    }
}


/**
 * Conversion RT90 2.5 gon V --> SWEREF 99 TM, i.e. "forward".
 */
Planar Transform::forward(const Planar& p) const
{
    return transform<PJ_FWD>(pj, p);
}


/**
 * Conversion SWEREF 99 TM ---> RT90 2.5 gon V.
 */
Planar Transform::backward(const Planar& p) const
{
    return transform<PJ_INV>(pj, p);
}


std::string Transform::pj_release()
{
    return proj_info().version;
}

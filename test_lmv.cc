/* The reference data at LMV must convert well.
 *
 * Copyright (c) 2012 Jörgen Grahn
 * All rights reserved.
 */
#include <testicle.h>

#include "planar.h"
#include "transform.h"
#include "lmv_ctrl.h"


namespace {

    /**
     * Assert that 'rt90' transforms to 'ref', with an error of at
     * most 0.1m.
     *
     * 0.1m is small enough to show dramatic fuckups, small enough not
     * to matter for my purposes ... and large enough not to trigger
     * on the ~1mm errors I've seen when playing with cs2cs(1)
     */
    void assert_from(const Planar& rt90, const Planar& ref)
    {
	Transform transform;
	const Planar p = transform.forward(rt90);
	testicle::assert_lt(distance2(p, ref), 0.1*0.1);
    }

    void assert_to(const Planar& sweref99, const Planar& ref)
    {
	Transform transform;
	const Planar p = transform.backward(sweref99);
	testicle::assert_lt(distance2(p, ref), 0.1*0.1);
    }
}


namespace lmv {

    void test_forward()
    {
	assert_from(lmv.rt90.A, lmv.sweref99.A);
	assert_from(lmv.rt90.B, lmv.sweref99.B);
	assert_from(lmv.rt90.C, lmv.sweref99.C);
	assert_from(lmv.rt90.D, lmv.sweref99.D);
    }

    void test_backward()
    {
	assert_to(lmv.sweref99.A, lmv.rt90.A);
	assert_to(lmv.sweref99.B, lmv.rt90.B);
	assert_to(lmv.sweref99.C, lmv.rt90.C);
	assert_to(lmv.sweref99.D, lmv.rt90.D);
    }
}

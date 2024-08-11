/* The reference data at LMV must convert well.
 *
 * Copyright (c) 2012 Jörgen Grahn
 * All rights reserved.
 */
#include <orchis.h>

#include "planar.h"
#include "transform.h"
#include "lmv_ctrl.h"


namespace {

    /* PROJ4 gave ~1mm errors; PROJ version 7 gives more than 0.1
     * meter. "Change is inevitable; progress is not."
     */
    void assert_near(const Planar& a, const Planar& b)
    {
	orchis::assert_lt(distance2(a, b), 0.2*0.2);
    }

    /**
     * Assert that 'rt90' transforms to 'ref', or something close.
     */
    void assert_from(const Planar& rt90, const Planar& ref)
    {
	Transform transform;
	const Planar p = transform.forward(rt90);
	assert_near(p, ref);
    }

    void assert_to(const Planar& sweref99, const Planar& ref)
    {
	Transform transform;
	const Planar p = transform.backward(sweref99);
	assert_near(p, ref);
    }
}


namespace lmv {

    void test_forward()
    {
	Transform::morons();
	assert_from(lmv.rt90.A, lmv.sweref99.A);
	assert_from(lmv.rt90.B, lmv.sweref99.B);
	assert_from(lmv.rt90.C, lmv.sweref99.C);
	assert_from(lmv.rt90.D, lmv.sweref99.D);
    }

    void test_backward()
    {
	Transform::morons();
	assert_to(lmv.sweref99.A, lmv.rt90.A);
	assert_to(lmv.sweref99.B, lmv.rt90.B);
	assert_to(lmv.sweref99.C, lmv.rt90.C);
	assert_to(lmv.sweref99.D, lmv.rt90.D);
    }
}

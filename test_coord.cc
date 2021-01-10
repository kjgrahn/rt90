/* The reference data at LMV must convert well.
 *
 * Copyright (c) 2012 Jörgen Grahn
 * All rights reserved.
 */
#include <orchis.h>

#include "coordinate.h"
#include "transform.h"

using orchis::assert_true;
using orchis::assert_false;
using orchis::assert_eq;

namespace accuracy {

    void test_empty()
    {
	Accuracy acc;
	assert_true(acc.empty());
	acc << 5;
	assert_false(acc.empty());
    }

    void test_tolerate()
    {
	Accuracy acc;
	acc << 6 << 4;

	assert_false(acc.tolerated(  999));
	assert_true( acc.tolerated( 1000));
	assert_true( acc.tolerated( 9999));
	assert_false(acc.tolerated(10000));

	assert_false(acc.tolerated(  99999));
	assert_true( acc.tolerated( 100000));
	assert_true( acc.tolerated( 999999));
	assert_false(acc.tolerated(9999999));
    }

    void test_format5()
    {
	Accuracy acc;
	acc << 6 << 5;

	assert_eq(acc.format(1000000), 10000);
	assert_eq(acc.format(2222222), 22222);
	assert_eq(acc.format(4444444), 44444);
	assert_eq(acc.format(5555500), 55555);
	assert_eq(acc.format(5555550), 55556);
	assert_eq(acc.format(5555555), 55556);
	assert_eq(acc.format(8888888), 88889);
    }

    void test_format7()
    {
	Accuracy acc;
	acc << 6 << 7;

	assert_eq(acc.format(1000000), 1000000);
	assert_eq(acc.format(2222222), 2222222);
	assert_eq(acc.format(4444444), 4444444);
	assert_eq(acc.format(5555500), 5555500);
	assert_eq(acc.format(5555550), 5555550);
	assert_eq(acc.format(5555555), 5555555);
	assert_eq(acc.format(8888888), 8888888);
    }
}


namespace rt90 {

    void test()
    {
	assert_eq(Rt90(7047738, 1522128),
		  Rt90(7047738, 1522128));
	assert_eq(Rt90(7047000, 1522000),
		  Rt90(704700,  152200));
	assert_eq(Rt90(7047000, 1522000),
		  Rt90(70470,   15220));
	assert_eq(Rt90(7047000, 1522000),
		  Rt90(7047,    1522));
    }

    void test_valid()
    {
	auto assert_valid = [] (const Rt90& a) { assert_true(a.valid()); };
	auto assert_invalid = [] (const Rt90& a) { assert_false(a.valid()); };

	assert_valid(Rt90(7047738, 1522128));
	assert_valid(Rt90(704773, 152212));
	assert_valid(Rt90(70477, 15221));
	assert_valid(Rt90(7047, 1522));

	assert_invalid(Rt90(7047738, 15221280));
	assert_invalid(Rt90(7047738, 152212));
	assert_invalid(Rt90(70477,   152212));
	assert_invalid(Rt90(70477,   1522));
	assert_invalid(Rt90(704,     1522));

	assert_invalid(Rt90(6047738, 1522128));
	assert_invalid(Rt90(8047738, 1522128));
    }
}


namespace conversion {

    void test()
    {
	static const Rt90 a(6671665, 1441843);
	static const Sweref99 b(6669189, 486557);

	const Transform t;
	assert_eq(convert(t, a), b);
	assert_eq(convert(t, b), a);
    }
}

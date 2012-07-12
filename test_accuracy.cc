/* The reference data at LMV must convert well.
 *
 * Copyright (c) 2012 Jörgen Grahn
 * All rights reserved.
 */
#include <testicle.h>

#include "coordinate.h"


namespace accuracy {

    void test_empty()
    {
	Accuracy acc;
	testicle::assert_(acc.empty());
	acc << 5;
	testicle::assert_(!acc.empty());
    }

    void test_tolerate()
    {
	Accuracy acc;
	acc << 6 << 4;

	testicle::assert_(!acc.tolerated(  999));
	testicle::assert_( acc.tolerated( 1000));
	testicle::assert_( acc.tolerated( 9999));
	testicle::assert_(!acc.tolerated(10000));

	testicle::assert_(!acc.tolerated(  99999));
	testicle::assert_( acc.tolerated( 100000));
	testicle::assert_( acc.tolerated( 999999));
	testicle::assert_(!acc.tolerated(9999999));
    }

    void test_format5()
    {
	Accuracy acc;
	acc << 6 << 5;

	testicle::assert_eq(acc.format(1000000), 10000);
	testicle::assert_eq(acc.format(2222222), 22222);
	testicle::assert_eq(acc.format(4444444), 44444);
	testicle::assert_eq(acc.format(5555500), 55555);
	testicle::assert_eq(acc.format(5555550), 55556);
	testicle::assert_eq(acc.format(5555555), 55556);
	testicle::assert_eq(acc.format(8888888), 88889);
    }

    void test_format7()
    {
	Accuracy acc;
	acc << 6 << 7;

	testicle::assert_eq(acc.format(1000000), 1000000);
	testicle::assert_eq(acc.format(2222222), 2222222);
	testicle::assert_eq(acc.format(4444444), 4444444);
	testicle::assert_eq(acc.format(5555500), 5555500);
	testicle::assert_eq(acc.format(5555550), 5555550);
	testicle::assert_eq(acc.format(5555555), 5555555);
	testicle::assert_eq(acc.format(8888888), 8888888);
    }
}

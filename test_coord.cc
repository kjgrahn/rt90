/* The reference data at LMV must convert well.
 *
 * Copyright (c) 2012 Jörgen Grahn
 * All rights reserved.
 */
#include <testicle.h>

#include "coordinate.h"
#include "transform.h"


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


namespace rt90 {

    void test()
    {
	testicle::assert_eq(Rt90(7047738, 1522128),
			    Rt90(7047738, 1522128));
	testicle::assert_eq(Rt90(7047000, 1522000),
			    Rt90(704700,  152200));
	testicle::assert_eq(Rt90(7047000, 1522000),
			    Rt90(70470,   15220));
	testicle::assert_eq(Rt90(7047000, 1522000),
			    Rt90(7047,    1522));
    }

    void test_valid()
    {
	testicle::assert_(Rt90(7047738, 1522128).valid());
	testicle::assert_(Rt90(704773, 152212).valid());
	testicle::assert_(Rt90(70477, 15221).valid());
	testicle::assert_(Rt90(7047, 1522).valid());

	testicle::assert_(!Rt90(7047738, 15221280).valid());
	testicle::assert_(!Rt90(7047738, 152212).valid());
	testicle::assert_(!Rt90(70477, 	 152212).valid());
	testicle::assert_(!Rt90(70477, 	 1522).valid());
	testicle::assert_(!Rt90(704,     1522).valid());

	testicle::assert_(!Rt90(6047738, 1522128).valid());
	testicle::assert_(!Rt90(8047738, 1522128).valid());
    }
}


namespace conversion {

    void test()
    {
	static const Rt90 a(6671665, 1441843);
	static const Sweref99 b(6669189, 486557);

	const Transform t;
	testicle::assert_eq(convert(t, a), b);
	testicle::assert_eq(convert(t, b), a);
    }
}

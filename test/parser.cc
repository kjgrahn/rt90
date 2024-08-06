/* The reference data at LMV must convert well.
 *
 * Copyright (c) 2012 Jörgen Grahn
 * All rights reserved.
 */
#include <orchis.h>

#include "coordinate.h"


namespace parser {

    void assert_parses(const Accuracy& acc,
		       bool rt90,
		       const char* s,
		       unsigned n,
		       unsigned e)
    {
	unsigned north;
	unsigned east;
	bool ok = parse(acc, rt90, s, north, east);
	orchis::assert_true(ok);
	orchis::assert_eq(n, north);
	orchis::assert_eq(e, east);
    }

    void assert_fails(const Accuracy& acc,
		      const char* s)
    {
	unsigned north;
	unsigned east;
	bool ok = parse(acc, true, s, north, east) || parse(acc, false, s, north, east);
	orchis::assert_false(ok);
    }

    void positive(orchis::TC)
    {
	Accuracy acc;
	acc << 5 << 7;

	assert_parses(acc, true,
		      "10000 10000",
		      10000, 10000);
	assert_parses(acc, true,
		      "99999 99999",
		      99999, 99999);
	assert_parses(acc, true,
		      " 99999   99999 \n",
		      99999, 99999);
	assert_parses(acc, true,
		      "9999999 9999999",
		      9999999, 9999999);
	assert_parses(acc, false,
		      "9999999 999999",
		      9999999, 999999);
    }

    void negative(orchis::TC)
    {
	Accuracy acc;
	acc << 5 << 7;

	assert_fails(acc, "");
	assert_fails(acc, "    \n");
	assert_fails(acc, "0x1869f 10000");
	assert_fails(acc, "10000 10000 42");
	assert_fails(acc, "  10000 0010000");
	assert_fails(acc, "  10000  01000");
	assert_fails(acc, " 100000 100000");
	assert_fails(acc, "100000 1000000");
	assert_fails(acc, "21000000 21000000");
    }

    namespace tolerant {

	void assert_parses(const std::string& s,
			   unsigned refn, unsigned refe)
	{
	    unsigned n;
	    unsigned e;
	    orchis::assert_true(parse(s, n, e));
	    orchis::assert_eq(n, refn);
	    orchis::assert_eq(e, refe);
	}

	void assert_fails(const std::string& s)
	{
	    unsigned n;
	    unsigned e;
	    orchis::assert_false(parse(s, n, e));
	}

        void positive(orchis::TC)
	{
	    assert_parses("1 2",   1, 2);
	    assert_parses("1x2",   1, 2);
	    assert_parses("x1x2x", 1, 2);
	    assert_parses("x1123455x23456789x",
			  1123455,
			  23456789);
	    assert_parses("---1123455---23456789---",
			  1123455,
			  23456789);
	}

	void swap(orchis::TC)
	{
	    assert_parses("E=136739 N=645220", 645220, 136739);
	    assert_parses("E=414796 N=6448948 ", 6448948, 414796);
	}

        void negative(orchis::TC)
	{
	    assert_fails("");
	    assert_fails("x");
	    assert_fails("0");
	    assert_fails("4711");
	    assert_fails("x4711x");
	    assert_fails("x4711xxx1xxxx2");
	}
    }
}

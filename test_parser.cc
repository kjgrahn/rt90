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

    void test_positive()
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

    void test_negative()
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
}

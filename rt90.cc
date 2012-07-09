/*
 * rt90 - coordinate conversion between RT90 and SWEREF99
 *
 * Copyright (c) 2012 Jörgen Grahn
 * All rights reserved.
 */

#include <string>
#include <iostream>

#include <getopt.h>

#include "transform.h"
#include "coordinate.h"
#include "lmv_ctrl.h"
#include "direction.h"


namespace {


    int convert(const Direction direction,
		const Accuracy& acc,
		const char* const x,
		const char* const y)
    {
    }


    int convert(const Direction direction,
		const Accuracy& acc,
		std::istream& is)
    {
    }


    /**
     * Perform the self-test, using LMV's four test points A--D.
     */
    int selftest()
    {
	int rc = 0;
	Transform t;
	const double limit = 0.1 * 0.1;

	using lmv::lmv;

	if(distance2(t.forward(lmv.rt90.A), lmv.sweref99.A) > limit) rc++;
	if(distance2(t.forward(lmv.rt90.B), lmv.sweref99.B) > limit) rc++;
	if(distance2(t.forward(lmv.rt90.C), lmv.sweref99.C) > limit) rc++;
	if(distance2(t.forward(lmv.rt90.D), lmv.sweref99.D) > limit) rc++;

	if(distance2(t.backward(lmv.sweref99.A), lmv.rt90.A) > limit) rc++;
	if(distance2(t.backward(lmv.sweref99.B), lmv.rt90.B) > limit) rc++;
	if(distance2(t.backward(lmv.sweref99.C), lmv.rt90.C) > limit) rc++;
	if(distance2(t.backward(lmv.sweref99.D), lmv.rt90.D) > limit) rc++;

	if(rc) {
	    std::cerr << "error: self-test failed\n";
	}

	return rc;
    }
}

int main(int argc, char ** argv)
{
    using std::string;

    const string prog = argv[0] ? argv[0] : "proj";
    const string usage = string("usage: ")
	+ prog + " [-4567] [north east]\n"
	+ "       "
	+ prog + " [--from | --to] [-4567] [north east]\n"
	+ "       "
	+ prog + " --test\n"
	+ "       "
	+ prog + " --version\n"
	+ "       "
	+ prog + " --help";

    const char optstring[] = "+4567";
    struct option long_options[] = {
	{"from",    0, 0, 'f'},
	{"to",      0, 0, 't'},
	{"back",    0, 0, 't'},
	{"test",    0, 0, 'T'},
	{"version", 0, 0, 'v'},
	{"help",    0, 0, 'h'},
	{0, 0, 0, 0}
    };

    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);

    Accuracy accuracy;
    Direction direction = BOTH_DIRECTIONS;
    bool test = false;

    int ch;
    while((ch = getopt_long(argc, argv,
			    optstring,
			    &long_options[0], 0)) != -1) {
	switch(ch) {
	case 'f':
	    direction = FROM_RT90; break;
	case 't':
	    direction = TO_RT90; break;
	case 'T':
	    test = true; break;
	case '4':
	case '5':
	case '6':
	case '7':
	    accuracy << (ch - '0');
	    break;
	case 'h':
	    std::cout << usage << '\n';
	    return 0;
	case 'v':
	    const char* version();
	    std::cout << prog << " version " << version() << "\n"
		      << "Copyright (c) 2012 Jörgen Grahn.\n"
		      << "Using PROJ.4 " << Transform::pj_release() << ".\n";
	    return 0;
	    break;
	case ':':
	case '?':
	    std::cerr << usage << '\n';
	    return 1;
	    break;
	default:
	    break;
	}
    }

    if(accuracy.empty()) {
	accuracy << 5 << 6 << 7;
    }

    if(test) {
	return selftest();
    }
    else if(argc - optind == 2) {
	return convert(direction, accuracy,
		       argv[optind], argv[optind+1]);
    }
    else if(argc - optind == 0) {
	return convert(direction, accuracy,
		       std::cin);
    }
    else {
	std::cerr << "error: argyment is not a coordinate\n";
	std::cerr << usage << '\n';
	return 1;
    }
}

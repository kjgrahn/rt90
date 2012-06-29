/*
 * rt90 - coordinate conversion between RT90 and SWEREF99
 *
 * Copyright (c) 2012 Jörgen Grahn
 * All rights reserved.
 */

#include <set>
#include <string>
#include <iostream>

#include <getopt.h>

#include "transform.h"


/**
 * RT90 accuracy for both input and output, i.e.  whether they
 * should be output in 4--7 digits (1km--1m), and which number of
 * digits should be tolerated.
 */
class Accuracy {
public:
    Accuracy& operator<< (unsigned n);
    bool empty() const { return input.empty(); }

private:
    unsigned output;
    std::set<unsigned> input;
};


Accuracy& Accuracy::operator<< (unsigned n)
{
    output = n;
    input.insert(n);
    return *this;
}


enum Direction {
    BOTH_DIRECTIONS,
    FROM_RT90,
    TO_RT90
};


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
    }
    else if(argc - optind == 2) {
    }
    else if(argc - optind == 0) {
    }
    else {
	std::cerr << "error: argyment is not a coordinate\n";
	std::cerr << usage << '\n';
	return 1;
    }
}

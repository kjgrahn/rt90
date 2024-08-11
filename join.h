/* Copyright (c) 2024 Jörgen Grahn
 * All rights reserved.
 */
#ifndef RT90_JOIN_H
#define RT90_JOIN_H

#include <sstream>
#include <iostream>

/**
 * Stream the elements of container 'c', separated by 'sep'.
 */
template <class Sep, class C>
std::ostream& join(std::ostream& os, const Sep& sep, const C& c)
{
    unsigned n = 0;
    for (const auto& elem : c) {
	if (n++) {
	    os << sep << elem;
	}
	else {
	    os << elem;
	}
    }
    return os;
}

template <class Sep, class C>
std::string join(const Sep& sep, const C& c)
{
    std::ostringstream oss;
    join(oss, sep, c);
    return oss.str();
}

#endif

#include <orchis.h>

#include <vector>
#include <string>
#include "join.h"

namespace joins {

    using Vc = std::vector<const char*>;
    using Vs = std::vector<std::string>;

    void empty(orchis::TC)
    {
	orchis::assert_eq(join(' ', Vc{}), "");
	orchis::assert_eq(join(' ', Vs{}), "");
    }

    void one(orchis::TC)
    {
	orchis::assert_eq(join(' ', Vc{"foo"}), "foo");
	orchis::assert_eq(join(' ', Vs{"foo"}), "foo");
    }

    void two(orchis::TC)
    {
	orchis::assert_eq(join(' ', Vc{"foo", "bar"}), "foo bar");
	orchis::assert_eq(join(' ', Vs{"foo", "bar"}), "foo bar");
    }

    void integer(orchis::TC)
    {
	using V = std::vector<int>;
	orchis::assert_eq(join('+', V{1, 13, 0}), "1+13+0");
	orchis::assert_eq(join(5, V{1, 13, 0}), "151350");
    }
}

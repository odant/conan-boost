/*
Copyright 2019 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/

#include <boost/core/use_default.hpp>

template<class, class = boost::use_default>
struct type { };

template struct type<int>;
template struct type<void, boost::use_default>;

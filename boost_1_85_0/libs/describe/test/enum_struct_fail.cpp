// Copyright 2021 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/describe/class.hpp>

#if !defined(BOOST_DESCRIBE_CXX14)

#error "Skipping test because C++14 is not available"

#else

enum E1 {};
BOOST_DESCRIBE_STRUCT(E1, (), ())

#endif // !defined(BOOST_DESCRIBE_CXX14)

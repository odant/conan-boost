///////////////////////////////////////////////////////////////
//  Copyright 2012 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#define NO_MIXED_OPS

#include <boost/multiprecision/cpp_dec_float.hpp>

#include "test_arithmetic.hpp"

template <unsigned D>
struct related_type<boost::multiprecision::number<boost::multiprecision::cpp_dec_float<D> > >
{
   typedef boost::multiprecision::number<boost::multiprecision::cpp_dec_float<D / 2> > type;
};

int main()
{
   test<boost::multiprecision::number<boost::multiprecision::cpp_dec_float<100, long long, std::allocator<char> >, boost::multiprecision::et_on> >();
   return boost::report_errors();
}

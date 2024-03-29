//  (C) Copyright Matt Borland 2021.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cmath>
#include <cfloat>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <boost/math/ccmath/isfinite.hpp>

#ifdef BOOST_HAS_FLOAT128
#include <boost/multiprecision/float128.hpp>
#endif

template <typename T>
void test()
{
    if constexpr (std::numeric_limits<T>::has_quiet_NaN)
    {
        static_assert(!boost::math::ccmath::isfinite(std::numeric_limits<T>::quiet_NaN()), "Wrong response to NAN");
    }

    static_assert(boost::math::ccmath::isfinite(T(0)), "Wrong response to 0");
    
    if constexpr (!std::is_integral_v<T>)
    {
        static_assert(boost::math::ccmath::isfinite((std::numeric_limits<T>::min)()/2), "Wrong response to subnormal");
        static_assert(!boost::math::ccmath::isfinite(std::numeric_limits<T>::infinity()), "Wrong response to infinity");
    }
    else
    {
        // Integer types define infinity as 0
        // https://en.cppreference.com/w/cpp/types/numeric_limits/infinity
        static_assert(boost::math::ccmath::isfinite(std::numeric_limits<T>::infinity()), "Wrong response to infinity");
    }
}

#ifndef BOOST_MATH_NO_CONSTEXPR_DETECTION
int main()
{
    test<float>();
    test<double>();

    #ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
    test<long double>();
    #endif
    
    #if defined(BOOST_MATH_TEST_FLOAT128) && !defined(BOOST_MATH_USING_BUILTIN_CONSTANT_P) && defined(BOOST_MATH_TEST_FLOAT128)
    test<boost::multiprecision::float128>();
    #endif

    test<int>();
    test<unsigned>();
    test<long>();
    test<std::int32_t>();
    test<std::int64_t>();
    test<std::uint32_t>();

    return 0;
}
#else
int main()
{
    return 0;
}
#endif

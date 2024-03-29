/*
Copyright 2020 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#include <boost/core/allocator_access.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <boost/core/detail/is_same.hpp>

template<class T>
struct A1 {
    typedef T value_type;
    typedef short difference_type;
};

template<class T>
struct A2 {
    typedef T value_type;
};

int main()
{
    BOOST_TEST_TRAIT_TRUE((boost::core::detail::is_same<short,
        boost::allocator_difference_type<A1<char> >::type>));
    BOOST_TEST_TRAIT_TRUE((boost::core::detail::is_same<std::ptrdiff_t,
        boost::allocator_difference_type<A2<int> >::type>));
    return boost::report_errors();
}

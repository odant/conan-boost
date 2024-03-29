// Copyright 2021 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/describe/enumerators.hpp>
#include <boost/describe/enum.hpp>
#include <boost/core/lightweight_test.hpp>

enum E1 {};
BOOST_DESCRIBE_ENUM(E1)

BOOST_DEFINE_ENUM(E2)

#if !defined(BOOST_DESCRIBE_CXX14)

#include <boost/config/pragma_message.hpp>

BOOST_PRAGMA_MESSAGE("Skipping test because C++14 is not available")
int main() {}

#else

#include <boost/mp11.hpp>
using namespace boost::mp11;

int main()
{
    {
        using D1 = boost::describe::describe_enumerators<E1>;
        BOOST_TEST_EQ( mp_size<D1>::value, 0 );
    }

    {
        using D2 = boost::describe::describe_enumerators<E2>;
        BOOST_TEST_EQ( mp_size<D2>::value, 0 );
    }

    return boost::report_errors();
}

#endif // !defined(BOOST_DESCRIBE_CXX14)

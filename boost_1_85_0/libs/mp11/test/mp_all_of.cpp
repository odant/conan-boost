
// Copyright 2015, 2016 Peter Dimov.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/mp11/detail/config.hpp>

#if BOOST_MP11_MSVC
# pragma warning( disable: 4503 ) // decorated name length exceeded
#endif

#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/list.hpp>
#include <boost/mp11/integral.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <type_traits>
#include <tuple>
#include <utility>

struct X1 {};

int main()
{
    using boost::mp11::mp_list;
    using boost::mp11::mp_all_of;
    using boost::mp11::mp_true;
    using boost::mp11::mp_false;

    {
        using L1 = mp_list<>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_all_of<L1, std::is_const>, mp_true>));

        using L2 = mp_list<X1 const, X1 const, X1 const volatile, X1 const, X1 const volatile>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_all_of<L2, std::is_volatile>, mp_false>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_all_of<L2, std::is_const>, mp_true>));
    }

    {
        using L1 = std::tuple<>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_all_of<L1, std::is_const>, mp_true>));

        using L2 = std::tuple<X1 const, X1 const, X1 const volatile, X1 const, X1 const volatile>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_all_of<L2, std::is_volatile>, mp_false>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_all_of<L2, std::is_const>, mp_true>));
    }

    {
        using L2 = std::pair<X1 const, X1 const volatile>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_all_of<L2, std::is_volatile>, mp_false>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_all_of<L2, std::is_const>, mp_true>));
    }

    {
        using boost::mp11::mp_repeat_c;
        using boost::mp11::mp_to_bool;
        using boost::mp11::mp_push_back;

        int const N = 1089;

        using L1 = mp_repeat_c<mp_list<mp_true>, N>;
        using R1 = mp_all_of<L1, mp_to_bool>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<R1, mp_true>));

        using L2 = mp_push_back<L1, mp_false>;
        using R2 = mp_all_of<L2, mp_to_bool>;

        BOOST_TEST_TRAIT_TRUE((std::is_same<R2, mp_false>));
    }

    return boost::report_errors();
}

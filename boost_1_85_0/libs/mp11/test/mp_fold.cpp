
//  Copyright 2015 Peter Dimov.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt


#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/list.hpp>
#include <boost/mp11/integral.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <type_traits>
#include <tuple>

struct X1 {};
struct X2 {};
struct X3 {};
struct X4 {};

template<class T1, class T2> struct F {};

using boost::mp11::mp_size_t;

struct W
{
    template<std::size_t N> void operator()( mp_size_t<N> ) const
    {
        using boost::mp11::mp_iota_c;
        using boost::mp11::mp_fold;
        using boost::mp11::mp_list;
        using boost::mp11::mp_push_back;
        using boost::mp11::mp_push_front;
        using boost::mp11::mp_reverse;
        using boost::mp11::mp_plus;

        using L = mp_iota_c<N>;

        using R1 = mp_fold<L, mp_list<>, mp_push_back>;
        BOOST_TEST_TRAIT_TRUE((std::is_same<R1, L>));

        using R2 = mp_fold<L, mp_list<>, mp_push_front>;
        BOOST_TEST_TRAIT_TRUE((std::is_same<R2, mp_reverse<L>>));

        using R3 = mp_fold<L, mp_size_t<0>, mp_plus>;
        BOOST_TEST_TRAIT_TRUE((std::is_same<R3, mp_size_t<N*(N-1)/2>>));
    }
};

int main()
{
    using boost::mp11::mp_list;
    using boost::mp11::mp_fold;

    {
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_fold<mp_list<>, void, F>, void>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_fold<mp_list<X1>, void, F>, F<void, X1>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_fold<mp_list<X1, X2>, void, F>, F<F<void, X1>, X2>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_fold<mp_list<X1, X2, X3>, void, F>, F<F<F<void, X1>, X2>, X3>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_fold<mp_list<X1, X2, X3, X4>, void, F>, F<F<F<F<void, X1>, X2>, X3>, X4>>));
    }

    {
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_fold<std::tuple<>, void, F>, void>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_fold<std::tuple<X1>, void, F>, F<void, X1>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_fold<std::tuple<X1, X2>, void, F>, F<F<void, X1>, X2>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_fold<std::tuple<X1, X2, X3>, void, F>, F<F<F<void, X1>, X2>, X3>>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_fold<std::tuple<X1, X2, X3, X4>, void, F>, F<F<F<F<void, X1>, X2>, X3>, X4>>));
    }

    using boost::mp11::mp_push_back;

    {
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_fold<std::tuple<X1, X2, X3, X4>, mp_list<>, mp_push_back>, mp_list<X1, X2, X3, X4>>));
    }

    using boost::mp11::mp_push_front;

    {
        BOOST_TEST_TRAIT_TRUE((std::is_same<mp_fold<std::tuple<X1, X2, X3, X4>, mp_list<>, mp_push_front>, mp_list<X4, X3, X2, X1>>));
    }

    using boost::mp11::mp_for_each;
    using boost::mp11::mp_iota_c;

#if BOOST_MP11_WORKAROUND( BOOST_MP11_MSVC, <= 1800 )

    W()( mp_size_t<37>() );

#else

    mp_for_each< mp_iota_c<38> >( W{} );

#endif

    return boost::report_errors();
}

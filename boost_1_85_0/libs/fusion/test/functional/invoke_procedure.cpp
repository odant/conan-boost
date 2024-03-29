/*=============================================================================
    Copyright (c) 2005-2006 Joao Abecasis
    Copyright (c) 2006-2007 Tobias Schwinger

    Use modification and distribution are subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#include <boost/config.hpp>
#include <boost/fusion/functional/invocation/invoke_procedure.hpp>
#include <boost/detail/lightweight_test.hpp>

#if !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
#include <functional>
#endif

#include <memory>
#include <boost/core/noncopyable.hpp>

#include <boost/mpl/int.hpp>

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/container/list.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/view/single_view.hpp>
#include <boost/fusion/view/iterator_range.hpp>
#include <boost/fusion/iterator/advance.hpp>
#include <boost/fusion/algorithm/transformation/join.hpp>

#include "../compile_time/sfinae_friendly.hpp"

namespace mpl = boost::mpl;
namespace fusion = boost::fusion;

template <typename T>
inline T const & const_(T const & t)
{
    return t;
}

struct object {};
struct object_nc : boost::noncopyable {};

typedef int         element1_type;
typedef object      element2_type;
typedef object_nc & element3_type;

int         element1 = 100;
object      element2 = object();
object_nc   element3;

class members
{
  public:
    int data;

    members()
        : data(20)
    { }

    int nullary() { return element1 = data + 1; }
    int nullary_c() const { return element1 = data + 2; }
    int unary(int & i) { return i = data + 3; }
    int unary_c(int & i) const { return i = data + 4; }
    int binary(int & i, object) { return i = data + 5; }
    int binary_c(int & i, object) const { return i = data + 6; }
};

#ifdef BOOST_NO_CXX11_SMART_PTR
typedef std::auto_ptr<members      >       members_ptr;
typedef std::auto_ptr<members const> const_members_ptr;
#else
typedef std::unique_ptr<members      >       members_ptr;
typedef std::unique_ptr<members const> const_members_ptr;
#endif

members that;
members_ptr spt_that(new members);
const_members_ptr spt_that_c(new members);

typedef fusion::single_view<members  > sv_obj;
typedef fusion::single_view<members &> sv_ref;
typedef fusion::single_view<members *> sv_ptr;
typedef fusion::single_view<members const  > sv_obj_c;
typedef fusion::single_view<members const &> sv_ref_c;
typedef fusion::single_view<members const *> sv_ptr_c;
typedef fusion::single_view<members_ptr const &> sv_spt;
typedef fusion::single_view<const_members_ptr const &> sv_spt_c;

sv_obj sv_obj_ctx(  that);
sv_ref sv_ref_ctx(  that);
sv_ptr sv_ptr_ctx(& that);
sv_obj_c sv_obj_c_ctx(  that);
sv_ref_c sv_ref_c_ctx(  that);
sv_ptr_c sv_ptr_c_ctx(& that);
sv_spt sv_spt_ctx(spt_that);
sv_spt_c sv_spt_c_ctx(spt_that_c);
template <typename F, typename S>
struct sv_helper
{
    SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<F, typename fusion::result_of::join<sv_obj  , S>::type>));
    SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<F, typename fusion::result_of::join<sv_ref  , S>::type>));
    SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<F, typename fusion::result_of::join<sv_ptr  , S>::type>));
    SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<F, typename fusion::result_of::join<sv_obj_c, S>::type>));
    SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<F, typename fusion::result_of::join<sv_ref_c, S>::type>));
    SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<F, typename fusion::result_of::join<sv_ptr_c, S>::type>));
    SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<F, typename fusion::result_of::join<sv_spt  , S>::type>));
    SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<F, typename fusion::result_of::join<sv_spt_c, S>::type>));
};
// FIXME:
//template struct sv_helper<int (members::*)()      , sfinae_friendly::v1>;
//template struct sv_helper<int (members::*)()      , sfinae_friendly::v2>;
//template struct sv_helper<int (members::*)()      , sfinae_friendly::v3>;
//template struct sv_helper<int (members::*)() const, sfinae_friendly::v1>;
//template struct sv_helper<int (members::*)() const, sfinae_friendly::v2>;
//template struct sv_helper<int (members::*)() const, sfinae_friendly::v3>;

//template struct sv_helper<int (members::*)(int)      , sfinae_friendly::v0>;
//template struct sv_helper<int (members::*)(int)      , sfinae_friendly::v1>;
//template struct sv_helper<int (members::*)(int)      , sfinae_friendly::v2>;
//template struct sv_helper<int (members::*)(int)      , sfinae_friendly::v3>;
//template struct sv_helper<int (members::*)(int) const, sfinae_friendly::v0>;
//template struct sv_helper<int (members::*)(int) const, sfinae_friendly::v1>;
//template struct sv_helper<int (members::*)(int) const, sfinae_friendly::v2>;
//template struct sv_helper<int (members::*)(int) const, sfinae_friendly::v3>;

//template struct sv_helper<int (members::*)(int, object)      , sfinae_friendly::v0>;
//template struct sv_helper<int (members::*)(int, object)      , sfinae_friendly::v1>;
//template struct sv_helper<int (members::*)(int, object)      , sfinae_friendly::v2>;
//template struct sv_helper<int (members::*)(int, object)      , sfinae_friendly::v3>;
//template struct sv_helper<int (members::*)(int, object) const, sfinae_friendly::v0>;
//template struct sv_helper<int (members::*)(int, object) const, sfinae_friendly::v1>;
//template struct sv_helper<int (members::*)(int, object) const, sfinae_friendly::v2>;
//template struct sv_helper<int (members::*)(int, object) const, sfinae_friendly::v3>;

struct fobj
{
    int operator()()       { return element1 = 0; }
    int operator()() const { return element1 = 1; }

    int operator()(int & i)       { return i = 2 ; }
    int operator()(int & i) const { return i = 3; }

    int operator()(int & i, object &)             { return i = 4; }
    int operator()(int & i, object &) const       { return i = 5; }
    int operator()(int & i, object const &)       { return i = 6; }
    int operator()(int & i, object const &) const { return i = 7; }

    int operator()(int & i, object &, object_nc &)       { return i = 10; }
    int operator()(int & i, object &, object_nc &) const { return i = 11; }
};
// FIXME:
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<fobj, sfinae_friendly::v0>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<fobj, sfinae_friendly::v1>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<fobj, sfinae_friendly::v2>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<fobj, sfinae_friendly::v3>));

struct fobj_nc
      : boost::noncopyable
{
    int operator()()       { return element1 = 12; }
    int operator()() const { return element1 = 13; }

    int operator()(int & i)       { return i = 14; }
    int operator()(int & i) const { return i = 15; }
};
// FIXME:
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<fobj_nc, sfinae_friendly::v0>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<fobj_nc, sfinae_friendly::v1>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<fobj_nc, sfinae_friendly::v2>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<fobj_nc, sfinae_friendly::v3>));

int nullary() { return element1 = 16; }
int unary(int & i) { return  i = 17; }
int binary1(int & i, object &) { return i = 18; }
int binary2(int & i, object const &) { return i = 19; }
//FIXME
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<int(*)(), sfinae_friendly::v1>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<int(*)(), sfinae_friendly::v2>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<int(*)(), sfinae_friendly::v3>));
//
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<int(*)(int), sfinae_friendly::v0>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<int(*)(int), sfinae_friendly::v1>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<int(*)(int), sfinae_friendly::v2>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<int(*)(int), sfinae_friendly::v3>));
//
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<int(*)(int, object &), sfinae_friendly::v0>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<int(*)(int, object &), sfinae_friendly::v1>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<int(*)(int, object &), sfinae_friendly::v2>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<int(*)(int, object &), sfinae_friendly::v3>));
//
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<int(*)(int, object const &), sfinae_friendly::v0>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<int(*)(int, object const &), sfinae_friendly::v1>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<int(*)(int, object const &), sfinae_friendly::v2>));
//SFINAE_FRIENDLY_ASSERT((fusion::result_of::invoke_procedure<int(*)(int, object const &), sfinae_friendly::v3>));

typedef int (*                   func_ptr)(int &);
typedef int (* const           c_func_ptr)(int &);
typedef int (* volatile        v_func_ptr)(int &);
typedef int (* const volatile cv_func_ptr)(int &);

   func_ptr func_ptr1 = &unary;
 c_func_ptr func_ptr2 = &unary;
 v_func_ptr func_ptr3 = &unary;
cv_func_ptr func_ptr4 = &unary;



#define COMPARE_EFFECT(e,t)               \
    {                                      \
        element1 = 1234567; e;             \
        int expected = element1;           \
        element1 = 1234567; t;             \
        BOOST_TEST(expected == element1 ); \
    }


template <class Sequence>
void test_sequence_n(Sequence & seq, mpl::int_<0>)
{
    // Function Objects

    fobj f;

    COMPARE_EFFECT(f (), fusion::invoke_procedure(f ,        seq ));
    COMPARE_EFFECT(f (), fusion::invoke_procedure(f , const_(seq)));

    // Note: The function object is taken by value, so we request the copy
    // to be const with an explicit template argument. We can also request
    // the function object to be pased by reference...
    COMPARE_EFFECT(const_(f)(), fusion::invoke_procedure<fobj const  >(const_(f),        seq ));
    COMPARE_EFFECT(const_(f)(), fusion::invoke_procedure<fobj const &>(const_(f), const_(seq)));

    fobj_nc nc_f;
    // ...and we further ensure there is no copying in this case, using a
    // noncopyable function object.
    COMPARE_EFFECT(nc_f (), fusion::invoke_procedure<fobj_nc &>(nc_f ,        seq ));
    COMPARE_EFFECT(nc_f (), fusion::invoke_procedure<fobj_nc &>(nc_f , const_(seq)));
    COMPARE_EFFECT(const_(nc_f)(), fusion::invoke_procedure<fobj_nc const &>(const_(nc_f),        seq ));
    COMPARE_EFFECT(const_(nc_f)(), fusion::invoke_procedure<fobj_nc const &>(const_(nc_f), const_(seq)));

    // Builtin Functions

    // Call through ref/ptr to function
    COMPARE_EFFECT(nullary(), fusion::invoke_procedure<int (&)()>(nullary, seq));
    COMPARE_EFFECT(nullary(), fusion::invoke_procedure(& nullary, seq));

    // Call through ptr to member function
    // Note: The non-const function members::nullary can't be invoked with
    // fusion::join(sv_obj_ctx,seq)), which is const and so is its first element
    COMPARE_EFFECT(that.nullary(), fusion::invoke_procedure(& members::nullary, fusion::join(sv_ref_ctx,seq)));
    COMPARE_EFFECT(that.nullary(), fusion::invoke_procedure(& members::nullary, fusion::join(sv_ptr_ctx,seq)));
    COMPARE_EFFECT(that.nullary(), fusion::invoke_procedure(& members::nullary, fusion::join(sv_spt_ctx,seq)));
    COMPARE_EFFECT(that.nullary_c(), fusion::invoke_procedure(& members::nullary_c, fusion::join(sv_obj_ctx,seq)));
    COMPARE_EFFECT(that.nullary_c(), fusion::invoke_procedure(& members::nullary_c, fusion::join(sv_ref_ctx,seq)));
    COMPARE_EFFECT(that.nullary_c(), fusion::invoke_procedure(& members::nullary_c, fusion::join(sv_ptr_ctx,seq)));
    COMPARE_EFFECT(that.nullary_c(), fusion::invoke_procedure(& members::nullary_c, fusion::join(sv_spt_ctx,seq)));
    COMPARE_EFFECT(that.nullary_c(), fusion::invoke_procedure(& members::nullary_c, fusion::join(sv_obj_c_ctx,seq)));
    COMPARE_EFFECT(that.nullary_c(), fusion::invoke_procedure(& members::nullary_c, fusion::join(sv_ref_c_ctx,seq)));
    COMPARE_EFFECT(that.nullary_c(), fusion::invoke_procedure(& members::nullary_c, fusion::join(sv_ptr_c_ctx,seq)));
    COMPARE_EFFECT(that.nullary_c(), fusion::invoke_procedure(& members::nullary_c, fusion::join(sv_spt_c_ctx,seq)));
}

template <class Sequence>
void test_sequence_n(Sequence & seq, mpl::int_<1>)
{
    fobj f;
    COMPARE_EFFECT(f(element1), fusion::invoke_procedure(f , seq ));
    COMPARE_EFFECT(f(element1), fusion::invoke_procedure(f , const_(seq)));
    COMPARE_EFFECT(const_(f)(element1), fusion::invoke_procedure<fobj const  >(const_(f), seq ));
    COMPARE_EFFECT(const_(f)(element1), fusion::invoke_procedure<fobj const &>(const_(f), const_(seq)));

    fobj_nc nc_f;
    COMPARE_EFFECT(nc_f(element1), fusion::invoke_procedure<fobj_nc &>(nc_f, seq ));
    COMPARE_EFFECT(nc_f(element1), fusion::invoke_procedure<fobj_nc &>(nc_f, const_(seq)));
    COMPARE_EFFECT(const_(nc_f)(element1), fusion::invoke_procedure<fobj_nc const &>(const_(nc_f), seq ));
    COMPARE_EFFECT(const_(nc_f)(element1), fusion::invoke_procedure<fobj_nc const &>(const_(nc_f), const_(seq)));

    COMPARE_EFFECT(unary(element1), fusion::invoke_procedure<int (&)(int &)>(unary, seq));
    COMPARE_EFFECT(func_ptr1(element1), fusion::invoke_procedure(func_ptr1, seq));
    COMPARE_EFFECT(func_ptr2(element1), fusion::invoke_procedure(func_ptr2, seq));
    COMPARE_EFFECT(func_ptr3(element1), fusion::invoke_procedure(func_ptr3, seq));
    COMPARE_EFFECT(func_ptr4(element1), fusion::invoke_procedure(func_ptr4, seq));

    COMPARE_EFFECT(that.unary(element1), fusion::invoke_procedure(& members::unary, fusion::join(sv_ref_ctx,seq)));
    COMPARE_EFFECT(that.unary(element1), fusion::invoke_procedure(& members::unary, fusion::join(sv_ptr_ctx,seq)));
    COMPARE_EFFECT(that.unary(element1), fusion::invoke_procedure(& members::unary, fusion::join(sv_spt_ctx,seq)));
    COMPARE_EFFECT(that.unary_c(element1), fusion::invoke_procedure(& members::unary_c, fusion::join(sv_obj_ctx,seq)));
    COMPARE_EFFECT(that.unary_c(element1), fusion::invoke_procedure(& members::unary_c, fusion::join(sv_ref_ctx,seq)));
    COMPARE_EFFECT(that.unary_c(element1), fusion::invoke_procedure(& members::unary_c, fusion::join(sv_ptr_ctx,seq)));
    COMPARE_EFFECT(that.unary_c(element1), fusion::invoke_procedure(& members::unary_c, fusion::join(sv_spt_ctx,seq)));
    COMPARE_EFFECT(that.unary_c(element1), fusion::invoke_procedure(& members::unary_c, fusion::join(sv_obj_c_ctx,seq)));
    COMPARE_EFFECT(that.unary_c(element1), fusion::invoke_procedure(& members::unary_c, fusion::join(sv_ref_c_ctx,seq)));
    COMPARE_EFFECT(that.unary_c(element1), fusion::invoke_procedure(& members::unary_c, fusion::join(sv_ptr_c_ctx,seq)));
    COMPARE_EFFECT(that.unary_c(element1), fusion::invoke_procedure(& members::unary_c, fusion::join(sv_spt_c_ctx,seq)));
}

template <class Sequence>
void test_sequence_n(Sequence & seq, mpl::int_<2>)
{
    fobj f;
    COMPARE_EFFECT(f (element1, element2), fusion::invoke_procedure(f , seq));
    COMPARE_EFFECT(f (element1, const_(element2)), fusion::invoke_procedure(f , const_(seq)));
    COMPARE_EFFECT(const_(f)(element1, element2), fusion::invoke_procedure<fobj const>(const_(f), seq));
    COMPARE_EFFECT(const_(f)(element1, const_(element2)), fusion::invoke_procedure<fobj const>(const_(f), const_(seq)));

    COMPARE_EFFECT(binary1(element1, element2), fusion::invoke_procedure(binary1, seq));
    COMPARE_EFFECT(binary2(element1, element2), fusion::invoke_procedure(binary2, seq));

    COMPARE_EFFECT(that.binary(element1,element2), fusion::invoke_procedure(& members::binary, fusion::join(sv_ref_ctx,seq)));
    COMPARE_EFFECT(that.binary(element1,element2), fusion::invoke_procedure(& members::binary, fusion::join(sv_ptr_ctx,seq)));
    COMPARE_EFFECT(that.binary(element1,element2), fusion::invoke_procedure(& members::binary, fusion::join(sv_spt_ctx,seq)));
    COMPARE_EFFECT(that.binary_c(element1,element2), fusion::invoke_procedure(& members::binary_c, fusion::join(sv_obj_ctx,seq)));
    COMPARE_EFFECT(that.binary_c(element1,element2), fusion::invoke_procedure(& members::binary_c, fusion::join(sv_ref_ctx,seq)));
    COMPARE_EFFECT(that.binary_c(element1,element2), fusion::invoke_procedure(& members::binary_c, fusion::join(sv_ptr_ctx,seq)));
    COMPARE_EFFECT(that.binary_c(element1,element2), fusion::invoke_procedure(& members::binary_c, fusion::join(sv_spt_ctx,seq)));
    COMPARE_EFFECT(that.binary_c(element1,element2), fusion::invoke_procedure(& members::binary_c, fusion::join(sv_obj_c_ctx,seq)));
    COMPARE_EFFECT(that.binary_c(element1,element2), fusion::invoke_procedure(& members::binary_c, fusion::join(sv_ref_c_ctx,seq)));
    COMPARE_EFFECT(that.binary_c(element1,element2), fusion::invoke_procedure(& members::binary_c, fusion::join(sv_ptr_c_ctx,seq)));
    COMPARE_EFFECT(that.binary_c(element1,element2), fusion::invoke_procedure(& members::binary_c, fusion::join(sv_spt_c_ctx,seq)));
}

template <class Sequence>
void test_sequence_n(Sequence & seq, mpl::int_<3>)
{
    fobj f;

    COMPARE_EFFECT(f(element1, element2, element3), fusion::invoke_procedure(f, seq));
    COMPARE_EFFECT(const_(f)(element1, element2, element3), fusion::invoke_procedure<fobj const>(const_(f), seq));
}

template <class Sequence>
void test_sequence(Sequence & seq)
{
    test_sequence_n(seq, mpl::int_<boost::fusion::result_of::size<Sequence>::value>());
}

int main()
{
    typedef fusion::vector<> vector0;
    typedef fusion::vector<element1_type &> vector1;
    typedef fusion::vector<element1_type &, element2_type> vector2;
    typedef fusion::vector<element1_type &, element2_type, element3_type> vector3;

    vector0 v0;
    vector1 v1(element1);

#if !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
    // Note: C++11 will pickup the rvalue overload for the d argument
    // since we do not have all permutations (expensive!) for all const&
    // and && arguments. We either have all && or all const& arguments only.
    // For that matter, use std::ref to disambiguate the call.

    vector2 v2(std::ref(element1), element2);
    vector3 v3(std::ref(element1), element2, std::ref(element3));
#else
    vector2 v2(element1, element2);
    vector3 v3(element1, element2, element3);
#endif

    test_sequence(v0);
    test_sequence(v1);
    test_sequence(v2);
    test_sequence(v3);

    typedef fusion::list<> list0;
    typedef fusion::list<element1_type &> list1;
    typedef fusion::list<element1_type &, element2_type> list2;
    typedef fusion::list<element1_type &, element2_type, element3_type> list3;

    list0 l0;
    list1 l1(element1);
    list2 l2(element1, element2);
    list3 l3(element1, element2, element3);

    test_sequence(l0);
    test_sequence(l1);
    test_sequence(l2);
    test_sequence(l3);

    return boost::report_errors();
}


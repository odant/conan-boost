//  (C) Copyright Frederic Bron 2009-2011.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// It would be nice to get rid of the unnamed namespace here,
// but for now we just turn off inspection reporting :(
// boostinspect:nounnamed

#ifndef TT_HAS_BINARY_OPERATORS_HPP
#define TT_HAS_BINARY_OPERATORS_HPP

#if defined(__GNUC__) && (__GNUC__*10000 + __GNUC_MINOR__*100 + __GNUC_PATCHLEVEL__ > 40900)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif


// test with one template parameter
#define TEST_T(TYPE,RESULT) BOOST_CHECK_INTEGRAL_CONSTANT((::boost::BOOST_TT_TRAIT_NAME<TYPE>::value), RESULT)
// test with one template parameter plus return value
#define TEST_TR(TYPE,RET,RESULT) BOOST_CHECK_INTEGRAL_CONSTANT((::boost::BOOST_TT_TRAIT_NAME<TYPE,TYPE,RET>::value), RESULT)
// test with two template parameters
#define TEST_TT(TYPE1,TYPE2,RESULT) BOOST_CHECK_INTEGRAL_CONSTANT((::boost::BOOST_TT_TRAIT_NAME<TYPE1,TYPE2>::value), RESULT)
// test with two template parameters plus return value
#define TEST_TTR(TYPE1,TYPE2,RET,RESULT) BOOST_CHECK_INTEGRAL_CONSTANT((::boost::BOOST_TT_TRAIT_NAME<TYPE1,TYPE2,RET>::value), RESULT)

namespace {

struct without { };

struct ret { };

struct internal { BOOST_TT_PROC ret operator BOOST_TT_TRAIT_OP (const internal&) const; };

struct external { };
inline BOOST_TT_PROC ret operator BOOST_TT_TRAIT_OP (const external&, const external&) {  return ret();  }

struct comma1_ret { };
struct ret_with_comma1 { BOOST_TT_PROC comma1_ret operator,(int); };

struct internal_comma1 { BOOST_TT_PROC ret_with_comma1 operator BOOST_TT_TRAIT_OP (const internal_comma1&) const; };

struct external_comma1 { };
BOOST_TT_PROC ret_with_comma1 operator BOOST_TT_TRAIT_OP (const external_comma1&, const external_comma1&) { return ret_with_comma1(); }

struct ret_with_comma2 { BOOST_TT_PROC void operator,(int); };

struct internal_comma2 { BOOST_TT_PROC ret_with_comma2 operator BOOST_TT_TRAIT_OP (const internal_comma2&) const; };

struct external_comma2 { };
BOOST_TT_PROC ret_with_comma2 operator BOOST_TT_TRAIT_OP (const external_comma2&, const external_comma2&){ return ret_with_comma2(); }

struct returns_int { BOOST_TT_PROC int operator BOOST_TT_TRAIT_OP (const returns_int&)const; };

struct returns_void { BOOST_TT_PROC void operator BOOST_TT_TRAIT_OP (const returns_void&)const; };

struct returns_void_star { BOOST_TT_PROC void *operator BOOST_TT_TRAIT_OP (const returns_void_star&)const; };

struct returns_double { BOOST_TT_PROC double operator BOOST_TT_TRAIT_OP (const returns_double&)const; };

struct ret1 { };
struct convertible_to_ret1 { BOOST_TT_PROC operator ret1 () const; };
struct returns_convertible_to_ret1 { BOOST_TT_PROC convertible_to_ret1 operator BOOST_TT_TRAIT_OP (const returns_convertible_to_ret1&)const; };

struct convertible_to_ret2 { };
struct ret2 { BOOST_TT_PROC ret2(const convertible_to_ret2); };
struct returns_convertible_to_ret2 { BOOST_TT_PROC convertible_to_ret2 operator BOOST_TT_TRAIT_OP (const returns_convertible_to_ret2&)const; };

class Base1 { };
class Derived1 : public Base1 { };

BOOST_TT_PROC bool operator BOOST_TT_TRAIT_OP (const Base1&, const Base1&) { return true; }

class Base2 { };
struct Derived2 : public Base2 {
   BOOST_TT_PROC Derived2(int); // to check if it works with a class that is not default constructible
};

BOOST_TT_PROC bool operator BOOST_TT_TRAIT_OP (const Derived2&, const Derived2&) { return true; }

struct tag { };

struct A { };
struct B : public A { };

struct C { };
struct D { };
BOOST_TT_PROC inline bool operator BOOST_TT_TRAIT_OP (const C&, void*) { return true; }
BOOST_TT_PROC inline bool operator BOOST_TT_TRAIT_OP (void*, const D&) { return true; }
BOOST_TT_PROC inline bool operator BOOST_TT_TRAIT_OP (const C&, const D&) { return true; }

struct private_op { private: BOOST_TT_PROC void operator BOOST_TT_TRAIT_OP (const private_op&) {} };

struct ambiguous_A 
{ 
};
inline BOOST_TT_PROC bool operator BOOST_TT_TRAIT_OP (const ambiguous_A&, const ambiguous_A&) { return true; }
struct ambiguous_B { BOOST_TT_PROC operator ambiguous_A()const { return ambiguous_A(); } };

//class internal_private { ret operator BOOST_TT_TRAIT_OP (const internal_private&) const; };

BOOST_TT_PROC void common() {
   TEST_T(void, false);
   TEST_TT(void, void, false);
   TEST_TTR(void, void, void, false);
   TEST_TTR(void, void, int, false);

   TEST_T(without, false);
   TEST_T(internal, true);
   TEST_T(external, true);
   TEST_T(internal_comma1, true);
   TEST_T(external_comma1, true);
   TEST_T(internal_comma2, true);
   TEST_T(external_comma2, true);
   TEST_T(returns_int, true);
   TEST_T(returns_void, true);
   TEST_T(returns_void_star, true);
   TEST_T(returns_double, true);
   TEST_T(returns_convertible_to_ret1, true);
   TEST_T(returns_convertible_to_ret2, true);
   TEST_T(Base1, true);
   TEST_T(Derived1, true);
   TEST_T(Base2, false);
   TEST_T(Derived2, true);

   TEST_TR(without, void, false);
   TEST_TR(without, bool, false);
   TEST_TR(internal, void, false);
   TEST_TR(internal, bool, false);
   TEST_TR(internal, ret, true);
   TEST_TR(internal_comma1, void, false);
   TEST_TR(internal_comma1, bool, false);
   TEST_TR(internal_comma1, ret_with_comma1, true);
   TEST_TR(internal_comma2, void, false);
   TEST_TR(internal_comma2, bool, false);
   TEST_TR(internal_comma2, ret_with_comma2, true);
   TEST_TR(external, void, false);
   TEST_TR(external, bool, false);
   TEST_TR(external, ret, true);
   TEST_TR(returns_int, void, false);
   TEST_TR(returns_int, bool, true);
   TEST_TR(returns_int, int, true);
   TEST_TR(returns_void, void, true);
   TEST_TR(returns_void, bool, false);
   TEST_TR(returns_void_star, bool, true);
   TEST_TR(returns_double, void, false);
   TEST_TR(returns_double, bool, true);
   TEST_TR(returns_double, double, true);
   TEST_TR(returns_convertible_to_ret1, void, false);
   TEST_TR(returns_convertible_to_ret1, ret1, true);
   TEST_TR(returns_convertible_to_ret2, ret2, true);
   TEST_TR(Base1, bool, true);
   TEST_TR(Derived1, bool, true);
   TEST_TR(Base2, bool, false);
   TEST_TR(Derived2, bool, true);
// compile time error
// TEST_T(internal_private, false);
#if defined(BOOST_TT_HAS_ACCURATE_BINARY_OPERATOR_DETECTION)
// There are some things that pass that wouldn't otherwise do so:
#if !BOOST_WORKAROUND(BOOST_MSVC, < 1910)
   TEST_TR(private_op, bool, false);
   TEST_T(private_op, false);
#endif
   TEST_TR(ambiguous_A, bool, true);
   TEST_T(ambiguous_A, true);
   TEST_TR(ambiguous_B, bool, true);
   TEST_T(ambiguous_B, true);
#endif
}

}

#if defined(__GNUC__) && (__GNUC__*10000 + __GNUC_MINOR__*100 + __GNUC_PATCHLEVEL__ > 40900)
#pragma GCC diagnostic pop
#endif

#endif


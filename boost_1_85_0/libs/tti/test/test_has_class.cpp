
//  (C) Copyright Edward Diener 2019
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#include "test_has_class.hpp"
#include <boost/detail/lightweight_test.hpp>

int main()
  {
  
  BOOST_TEST(!BOOST_TTI_HAS_CLASS_GEN(AnIntType)<AType>::value);
  BOOST_TEST(!BOOST_TTI_HAS_CLASS_GEN(AnIntTypeReference)<AType>::value);
  BOOST_TEST(NameStruct<AType>::value);
  BOOST_TEST(BOOST_TTI_HAS_CLASS_GEN(BType)<AType>::value);
  BOOST_TEST(!TheInteger<AType::BType>::value);
  BOOST_TEST(BOOST_TTI_HAS_CLASS_GEN(CType)<AType::BType>::value);
  BOOST_TEST(!BOOST_TTI_HAS_CLASS_GEN(AnotherIntegerType)<AType::BType::CType>::value);
  BOOST_TEST(!SomethingElse<AnotherType>::value);
  BOOST_TEST(!BOOST_TTI_HAS_CLASS_GEN(NoOtherType)<AnotherType>::value);
  
  BOOST_TEST(!EInB<AType::BType>::value);
  BOOST_TEST(!BOOST_TTI_HAS_CLASS_GEN(AnEnumTtype)<AType>::value);
  BOOST_TEST(!AnotherE<AnotherType>::value);
  BOOST_TEST(!EClass<AType>::value);
  BOOST_TEST(!BOOST_TTI_HAS_CLASS_GEN(AnotherEnumClassType)<AnotherType>::value);
  BOOST_TEST(!BOOST_TTI_HAS_CLASS_GEN(CTypeUnion)<AType::BType::CType>::value);
  BOOST_TEST(!SimpleUT<AType>::value);
  BOOST_TEST(!BOOST_TTI_HAS_CLASS_GEN(AnotherUnion)<AnotherType>::value);
  
  BOOST_TEST(UnionType<AType::AnUnion>::value);
  
  // Passing non-class enclosing type will return false
  
  BOOST_TEST(!BOOST_TTI_HAS_CLASS_GEN(AnIntTypeReference)<signed long>::value);
  BOOST_TEST(!NameStruct<AType &>::value);
  
  return boost::report_errors();

  }


//  (C) Copyright Edward Diener 2011
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#include "test_has_mem_fun.hpp"
#include <boost/detail/lightweight_test.hpp>

int main()
  {
  
  BOOST_TEST((BOOST_TTI_HAS_MEMBER_FUNCTION_GEN(VoidFunction)<AType,void>::value));
  BOOST_TEST((FunctionReturningInt<AType,int>::value));
  BOOST_TEST((FunctionReturningInt<AnotherType,double,boost::mpl::vector<int> >::value));
  BOOST_TEST((BOOST_TTI_HAS_MEMBER_FUNCTION_GEN(aFunction)<AnotherType,AType,boost::mpl::vector<int> >::value));
  BOOST_TEST((AnotherIntFunction<AnotherType,int,boost::mpl::vector<AType> >::value));
  BOOST_TEST((BOOST_TTI_HAS_MEMBER_FUNCTION_GEN(sFunction)<AnotherType,AType::AnIntType,boost::mpl::vector<int,long,double> >::value));
  BOOST_TEST((!BOOST_TTI_HAS_MEMBER_FUNCTION_GEN(someFunctionMember)<AnotherType,AType,boost::mpl::vector<long,int> >::value));
  
  BOOST_TEST((BOOST_TTI_HAS_MEMBER_FUNCTION_GEN(CMemberFunction)<AType::BType::CType::CTypeUnion,void,boost::mpl::vector<int> >::value));
  
  // Test enclosing class as non-class type
  
  BOOST_TEST((!AnotherIntFunction<short,int,boost::mpl::vector<AType> >::value));
  BOOST_TEST((!BOOST_TTI_HAS_MEMBER_FUNCTION_GEN(VoidFunction)<unsigned,void>::value));
  BOOST_TEST((!BOOST_TTI_HAS_MEMBER_FUNCTION_GEN(sFunction)<AnotherType *,AType::AnIntType,boost::mpl::vector<int,long,double> >::value));
  BOOST_TEST((!FunctionReturningInt<float ***,int>::value));
  
  return boost::report_errors();

  }

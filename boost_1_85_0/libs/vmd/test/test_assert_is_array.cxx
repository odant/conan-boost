
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#include <boost/vmd/assert_is_array.hpp>
#include <boost/detail/lightweight_test.hpp>

int main()
  {
  
#if BOOST_PP_VARIADICS

  #define AN_ARRAY (7,(5,7,f,x,%,#,U))
#if !BOOST_VMD_MSVC_V8  
  #define AN_EMPTY_ARRAY (0,())
#endif
  
  BOOST_VMD_ASSERT_IS_ARRAY((4,(x,3,e,2)))
  BOOST_VMD_ASSERT_IS_ARRAY((6,(x,3,e,2,(4,(x,3,e,2)),%)))
  BOOST_VMD_ASSERT_IS_ARRAY(AN_ARRAY)
#if !BOOST_VMD_MSVC_V8  
  BOOST_VMD_ASSERT_IS_ARRAY(AN_EMPTY_ARRAY)
#endif
  
#else

  BOOST_VMD_ASSERT(0)
  
#endif

  return boost::report_errors();
  
  }

// Copyright 2002 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Boost.MultiArray Library
//  Authors: Ronald Garcia
//           Jeremy Siek
//           Andrew Lumsdaine
//  See http://www.boost.org/libs/multi_array for documentation.

// 
// fail_csubarray2.cpp
//   Testing constness of subarray operations.
//

#include <boost/multi_array.hpp>

#include <boost/core/lightweight_test.hpp>

#include <boost/array.hpp>

int
main()
{
  const int ndims=3;
  typedef boost::multi_array<int,ndims> array;

  boost::array<array::size_type,ndims> sma_dims = {{2,3,4}};
  array sma(sma_dims);

  int num = 0;
  for (array::index i = 0; i != 2; ++i)
    for (array::index j = 0; j != 3; ++j)
      for (array::index k = 0; k != 4; ++k)
        sma[i][j][k] = num++;

  const array& sma_const = sma;

  // FAIL! preserve constness.
  array::subarray<ndims-1>::type sba = sma_const[0];

  return boost::report_errors();
}

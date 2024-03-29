//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2007-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#include "node_pool_test.hpp"
#include <boost/interprocess/allocators/detail/node_pool.hpp>

using namespace boost::interprocess;

typedef managed_shared_memory::segment_manager segment_manager_t;

int main ()
{
   typedef ipcdetail::private_node_pool
      <segment_manager_t, 4, 64> node_pool_t;

   if(!test::test_all_node_pool<node_pool_t>())
      return 1;

   return 0;
}

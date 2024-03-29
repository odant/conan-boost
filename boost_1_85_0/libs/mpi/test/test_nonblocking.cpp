// Copyright (C) 2006 Douglas Gregor.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// A test of the nonblocking point-to-point operations.
#include <boost/mpi/nonblocking.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include "gps_position.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/list.hpp>
#include <iterator>
#include <algorithm>
//#include "debugger.cpp"

#include "mpi_test_utils.hpp"

using boost::mpi::communicator;
using boost::mpi::request;
using boost::mpi::status;

enum method_kind { 
  mk_wait_any, mk_test_any, mk_wait_all, mk_wait_all_keep, 
  mk_test_all, mk_test_all_keep, mk_wait_some, mk_wait_some_keep,
  mk_test_some, mk_test_some_keep,
  mk_test_size
};

static const char* method_kind_names[mk_test_size] = {
  "wait_any",
  "test_any",
  "wait_all",
  "wait_all (keep results)",
  "test_all",
  "test_all (keep results)",
  "wait_some",
  "wait_some (keep results)",
  "test_some",
  "test_some (keep results)"
};


template<typename T>
int
nonblocking_tests( const communicator& comm, const T* values, int num_values, 
                   const char* kind, bool composite)
{
  int failed = 0;
  BOOST_MPI_COUNT_FAILED(nonblocking_test(comm, values, num_values, kind, mk_wait_any), failed);
  BOOST_MPI_COUNT_FAILED(nonblocking_test(comm, values, num_values, kind, mk_test_any), failed);
  //wait_for_debugger(comm);
  BOOST_MPI_COUNT_FAILED(nonblocking_test(comm, values, num_values, kind, mk_wait_all), failed);
  BOOST_MPI_COUNT_FAILED(nonblocking_test(comm, values, num_values, kind, mk_wait_all_keep), failed);
  if (!composite) {
    BOOST_MPI_COUNT_FAILED(nonblocking_test(comm, values, num_values, kind, mk_test_all), failed);
    BOOST_MPI_COUNT_FAILED(nonblocking_test(comm, values, num_values, kind, mk_test_all_keep), failed);
  }
  BOOST_MPI_COUNT_FAILED(nonblocking_test(comm, values, num_values, kind, mk_wait_some), failed);
  BOOST_MPI_COUNT_FAILED(nonblocking_test(comm, values, num_values, kind, mk_wait_some_keep), failed);
  BOOST_MPI_COUNT_FAILED(nonblocking_test(comm, values, num_values, kind, mk_test_some), failed);
  BOOST_MPI_COUNT_FAILED(nonblocking_test(comm, values, num_values, kind, mk_test_some_keep), failed);

  return failed;
}

template<typename T>
int
nonblocking_test(const communicator& comm, const T* values, int num_values, 
                 const char* kind, method_kind method)
{
  using boost::mpi::wait_any;
  using boost::mpi::test_any;
  using boost::mpi::wait_all;
  using boost::mpi::test_all;
  using boost::mpi::wait_some;
  using boost::mpi::test_some;
  
  int failed = 0;
  
  int next = (comm.rank() + 1) % comm.size();
  int prev = (comm.rank() + comm.size() - 1) % comm.size();
			    
  if (comm.rank() == 0) {
    std::cout << "Testing " << method_kind_names[method] 
              << " with " << kind << "...";
    std::cout.flush();
  }
  
  typedef std::pair<status, std::vector<request>::iterator> 
    status_iterator_pair;
  
  T incoming_value;
  std::vector<T> incoming_values(num_values);
  std::vector<request> reqs;
  // Send/receive the first value
  reqs.push_back(comm.isend(next, 0, values[0]));
  reqs.push_back(comm.irecv(prev, 0, incoming_value));
  
  if (method != mk_wait_any && method != mk_test_any) {
#ifndef LAM_MPI
    // We've run into problems here (with 0-length messages) with
    // LAM/MPI on Mac OS X and x86-86 Linux. Will investigate
    // further at a later time, but the problem only seems to occur
    // when using shared memory, not TCP.
    
    // Send/receive an empty message
    reqs.push_back(comm.isend(next, 1));
    reqs.push_back(comm.irecv(prev, 1));
#endif
    
    // Send/receive an array
    reqs.push_back(comm.isend(next, 2, values, num_values));
    reqs.push_back(comm.irecv(prev, 2, &incoming_values.front(), num_values));
  }
  
  switch (method) {
  case mk_wait_any:
    if (wait_any(reqs.begin(), reqs.end()).second == reqs.begin())
      reqs[1].wait();
    else
      reqs[0].wait();
    break;
    
  case mk_test_any:
    {
      boost::optional<status_iterator_pair> result;
      do {
        result = test_any(reqs.begin(), reqs.end());
      } while (!result);
      if (result->second == reqs.begin())
        reqs[1].wait();
      else
        reqs[0].wait();
      break;
    }
    
  case mk_wait_all:
    wait_all(reqs.begin(), reqs.end());
    break;
    
  case mk_wait_all_keep:
    {
      std::vector<status> stats;
      wait_all(reqs.begin(), reqs.end(), std::back_inserter(stats));
    }
    break;
    
  case mk_test_all:
    while (!test_all(reqs.begin(), reqs.end())) { /* Busy wait */ }
    break;
    
  case mk_test_all_keep:
    {
      std::vector<status> stats;
      while (!test_all(reqs.begin(), reqs.end(), std::back_inserter(stats)))
        /* Busy wait */;
    }
    break;
    
  case mk_wait_some:
    {
      std::vector<request>::iterator pos = reqs.end();
      do {
        pos = wait_some(reqs.begin(), pos);
      } while (pos != reqs.begin());
    }
    break;
    
  case mk_wait_some_keep:
    {
      std::vector<status> stats;
      std::vector<request>::iterator pos = reqs.end();
      do {
        pos = wait_some(reqs.begin(), pos, std::back_inserter(stats)).second;
      } while (pos != reqs.begin());
    }
    break;
    
  case mk_test_some:
    {
      std::vector<request>::iterator pos = reqs.end();
      do {
        pos = test_some(reqs.begin(), pos);
      } while (pos != reqs.begin());
    }
    break;
    
  case mk_test_some_keep:
    {
      std::vector<status> stats;
      std::vector<request>::iterator pos = reqs.end();
      do {
        pos = test_some(reqs.begin(), pos, std::back_inserter(stats)).second;
      } while (pos != reqs.begin());
    }
    break;
    
  default:
    BOOST_MPI_CHECK(false, failed);
  }
  
  if (comm.rank() == 0) {
    bool okay = true;
    
    if (!((incoming_value == values[0])))
      okay = false;
    
    if (method != mk_wait_any && method != mk_test_any
        && !std::equal(incoming_values.begin(), incoming_values.end(),
                       values))
      okay = false;
    
    if (okay)
      std::cout << "OK." << std::endl;
    else
      std::cerr << "ERROR!" << std::endl;
  }
  
  BOOST_MPI_CHECK(incoming_value == values[0], failed);
  
  if (method != mk_wait_any && method != mk_test_any)
    BOOST_MPI_CHECK(std::equal(incoming_values.begin(), incoming_values.end(), values), failed);
  return failed;
}

int main() 
{
  boost::mpi::environment env;
  communicator comm;
  int failed = 0;
  int int_array[3] = {17, 42, 256};
  BOOST_MPI_COUNT_FAILED(nonblocking_tests(comm, int_array, 3, "integers", false), failed);

  gps_position gps_array[2] = {
    gps_position(17, 42, .06),
    gps_position(42, 17, .06)
  };
  BOOST_MPI_COUNT_FAILED(nonblocking_tests(comm, gps_array, 2, "gps positions", false), failed);

  std::string string_array[2] = { "Hello", "World" };
  BOOST_MPI_COUNT_FAILED(nonblocking_tests(comm, string_array, 2, "strings", true), failed);

  std::list<std::string> lst_of_strings;
  for (int i = 0; i < comm.size(); ++i)
    lst_of_strings.push_back(boost::lexical_cast<std::string>(i));

  BOOST_MPI_COUNT_FAILED(nonblocking_tests(comm, &lst_of_strings, 1, "list of strings", true), failed);
  return failed;
}

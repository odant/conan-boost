//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/move for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_MOVE_TEST_COPYMOVABLE_HPP
#define BOOST_MOVE_TEST_COPYMOVABLE_HPP

//[copy_movable_definition 
//header file "copymovable.hpp"
#include <boost/move/core.hpp>

//A copy_movable class
class copy_movable
{
   BOOST_COPYABLE_AND_MOVABLE(copy_movable)
   int value_;

   public:
   copy_movable() : value_(1){}

   //Move constructor and assignment
   copy_movable(BOOST_RV_REF(copy_movable) m)
   {  value_ = m.value_;   m.value_ = 0;  }

   copy_movable(const copy_movable &m)
   {  value_ = m.value_;   }

   copy_movable & operator=(BOOST_RV_REF(copy_movable) m)
   {  value_ = m.value_;   m.value_ = 0;  return *this;  }

   copy_movable & operator=(BOOST_COPY_ASSIGN_REF(copy_movable) m)
   {  value_ = m.value_;   return *this;  }

   bool moved() const //Observer
   {  return value_ == 0; }
};

//A copyable-only class
class copyable
{};

//A copyable-only class
class non_copy_movable
{
   public:
   non_copy_movable(){}
   private:
   non_copy_movable(const non_copy_movable&);
   non_copy_movable& operator=(const non_copy_movable&);
};

//]

#endif //BOOST_MOVE_TEST_COPYMOVABLE_HPP

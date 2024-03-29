/* Used in Boost.MultiIndex tests.
 *
 * Copyright 2003-2020 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef BOOST_MULTI_INDEX_TEST_NON_STD_ALLOCATOR_HPP
#define BOOST_MULTI_INDEX_TEST_NON_STD_ALLOCATOR_HPP

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/throw_exception.hpp>
#include <iterator>
#include <cstddef>
#include <stdexcept>

template<typename T>
class non_raw_pointer
{
public:
  typedef std::ptrdiff_t                  difference_type;
  typedef T                               value_type;
  typedef T*                              pointer;
  typedef T&                              reference;
  typedef std::random_access_iterator_tag iterator_category;

  non_raw_pointer(){}
  explicit non_raw_pointer(T* p_):p(p_){}

  T& operator*()const
  {
#if !defined(BOOST_NO_EXCEPTIONS)
    if(!p)boost::throw_exception(std::runtime_error("null indirection"));
#endif

    return *p;
  }

  T* operator->()const{return p;}
  non_raw_pointer& operator++(){++p;return *this;}
  non_raw_pointer operator++(int){non_raw_pointer t(*this);++p;return t;}
  non_raw_pointer& operator--(){--p;return *this;}
  non_raw_pointer operator--(int){non_raw_pointer t(*this);--p;return t;}
  non_raw_pointer& operator+=(std::ptrdiff_t n){p+=n;return *this;}
  non_raw_pointer& operator-=(std::ptrdiff_t n){p-=n;return *this;}
  T& operator[](std::ptrdiff_t n)const{return p[n];}

  T* raw_ptr()const{return p;}

private:
  T* p;
};

template<typename T>
non_raw_pointer<T> operator+(const non_raw_pointer<T>& x,std::ptrdiff_t n)
{return non_raw_pointer<T>(x.raw_ptr()+n);}

template<typename T>
non_raw_pointer<T> operator+(std::ptrdiff_t n,const non_raw_pointer<T>& x)
{return non_raw_pointer<T>(n+x.raw_ptr());}

template<typename T>
non_raw_pointer<T> operator-(const non_raw_pointer<T>& x,std::ptrdiff_t n)
{return non_raw_pointer<T>(x.raw_ptr()-n);}

template<typename T>
std::ptrdiff_t operator-(
  const non_raw_pointer<T>& x,const non_raw_pointer<T>& y)
{return x.raw_ptr()-y.raw_ptr();}

template<typename T>
bool operator==(const non_raw_pointer<T>& x,const non_raw_pointer<T>& y)
{return x.raw_ptr()==y.raw_ptr();}

template<typename T>
bool operator!=(const non_raw_pointer<T>& x,const non_raw_pointer<T>& y)
{return x.raw_ptr()!=y.raw_ptr();}

template<typename T>
bool operator<(const non_raw_pointer<T>& x,const non_raw_pointer<T>& y)
{return x.raw_ptr()<y.raw_ptr();}

template<typename T>
bool operator>(const non_raw_pointer<T>& x,const non_raw_pointer<T>& y)
{return x.raw_ptr()>y.raw_ptr();}

template<typename T>
bool operator>=(const non_raw_pointer<T>& x,const non_raw_pointer<T>& y)
{return x.raw_ptr()>=y.raw_ptr();}

template<typename T>
bool operator<=(const non_raw_pointer<T>& x,const non_raw_pointer<T>& y)
{return x.raw_ptr()<=y.raw_ptr();}

template<typename T>
class non_std_allocator
{
public:
  typedef std::size_t              size_type;
  typedef std::ptrdiff_t           difference_type;
  typedef non_raw_pointer<T>       pointer;
  typedef non_raw_pointer<const T> const_pointer;
  typedef void*                    void_pointer;
  typedef const void*              const_void_pointer;
  typedef T&                       reference;
  typedef const T&                 const_reference;
  typedef T                        value_type;
  template<class U>struct rebind{typedef non_std_allocator<U> other;};

  non_std_allocator(int id_=0):id(id_){}
  non_std_allocator(const non_std_allocator<T>& x):id(x.id){}
  template<class U>non_std_allocator(const non_std_allocator<U>& x,int=0):
    id(x.id){}
  non_std_allocator& operator=(const non_std_allocator<T>& x)
    {id=x.id; return *this;}

  pointer allocate(size_type n)
  {
    return pointer((T*)(new char[n*sizeof(T)]));
  }

  void deallocate(pointer p,size_type)
  {
    delete[](char *)&*p;
  }

  size_type max_size() const{return (size_type )(-1);}

  friend bool operator==(const non_std_allocator& x,const non_std_allocator& y)
  {
    return x.id==y.id;
  }

  friend bool operator!=(const non_std_allocator& x,const non_std_allocator& y)
  {
    return !(x==y);
  }

  int id;
};

#endif

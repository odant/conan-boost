//  (C) Copyright Beman Dawes 2009

//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for more information.

//  MACRO:         BOOST_NO_CXX11_HDR_TYPE_TRAITS
//  TITLE:         C++0x header <type_traits> unavailable
//  DESCRIPTION:   The standard library does not supply C++0x header <type_traits>

#include <type_traits>

namespace boost_no_cxx11_hdr_type_traits {

int test()
{
  using std::integral_constant;
  using std::true_type;
  using std::false_type;
  using std::is_void;
  using std::is_integral;
  using std::is_floating_point;
  using std::is_array;
  using std::is_pointer;
  using std::is_lvalue_reference;
  using std::is_rvalue_reference;
  using std::is_member_object_pointer;
  using std::is_member_function_pointer;
  using std::is_enum;
  using std::is_union;
  using std::is_class;
  using std::is_function;
  using std::is_reference;
  using std::is_arithmetic;
  using std::is_fundamental;
  using std::is_object;
  using std::is_scalar;
  using std::is_compound;
  using std::is_member_pointer;
  using std::is_const;
  using std::is_volatile;
  using std::is_trivial;
  using std::is_trivially_copyable;
  using std::is_standard_layout;
#if !((__cplusplus > 201703) || (defined(_MSVC_LANG) && (_MSVC_LANG > 201703)))
  // deprecated in C++20 (including preview editions):
  using std::is_pod;
#endif
#if !((__cplusplus > 201700) || (defined(_MSVC_LANG) && (_MSVC_LANG > 201700)))
  // deprecated in C++ 17:
  using std::is_literal_type;
  using std::result_of;
#endif
  using std::is_empty;
  using std::is_polymorphic;
  using std::is_abstract;
  using std::is_constructible;
  using std::is_nothrow_constructible;
  using std::is_default_constructible;
  using std::is_copy_constructible;
  using std::is_copy_assignable;
  using std::is_move_constructible;
  using std::is_move_assignable;
  using std::is_destructible;
  using std::is_trivially_default_constructible;
  using std::is_trivially_copy_constructible;
  using std::is_trivially_move_constructible;
  using std::is_trivially_copy_assignable;
  using std::is_trivially_move_assignable;
  using std::is_trivially_destructible;
  using std::is_nothrow_default_constructible;
  using std::is_nothrow_copy_constructible;
  using std::is_nothrow_move_constructible;
  using std::is_nothrow_copy_assignable;
  using std::is_nothrow_move_assignable;
  using std::has_virtual_destructor;
  using std::is_signed;
  using std::is_unsigned;
  using std::alignment_of;
  using std::rank;
  using std::extent;
  using std::is_same;
  using std::is_base_of;
  using std::is_convertible;
  using std::remove_const;
  using std::remove_volatile;
  using std::remove_cv;
  using std::add_const;
  using std::add_volatile;
  using std::add_cv;
  using std::remove_reference;
  using std::add_lvalue_reference;
  using std::add_rvalue_reference;
  using std::make_signed;
  using std::make_unsigned;
  using std::remove_extent;
  using std::remove_all_extents;
  using std::remove_pointer;
  using std::add_pointer;
#if !((__cplusplus > 202002L) || (defined(_MSVC_LANG) && (_MSVC_LANG > 202002L)))
  // deprecated in C++23:
  using std::aligned_storage;
  using std::aligned_union;
#endif
  using std::decay;
  using std::enable_if;
  using std::conditional;
  using std::common_type;
  using std::underlying_type;
  return 0;
}

}

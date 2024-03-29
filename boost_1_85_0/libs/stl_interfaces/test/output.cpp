// Copyright (C) 2019 T. Zachary Laine
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/stl_interfaces/iterator_interface.hpp>

#include <boost/core/lightweight_test.hpp>

#include <array>
#include <numeric>
#include <vector>
#include <type_traits>


struct basic_output_iter
    : boost::stl_interfaces::iterator_interface<
#if !BOOST_STL_INTERFACES_USE_DEDUCED_THIS
          basic_output_iter,
#endif
          std::output_iterator_tag, int>
{
    basic_output_iter() : it_(nullptr) {}
    basic_output_iter(int * it) : it_(it) {}

    int & operator*() noexcept { return *it_; }
    basic_output_iter & operator++() noexcept
    {
        ++it_;
        return *this;
    }

    using base_type = boost::stl_interfaces::iterator_interface<
#if !BOOST_STL_INTERFACES_USE_DEDUCED_THIS
        basic_output_iter,
#endif
        std::output_iterator_tag, int>;
    using base_type::operator++;

private:
    int * it_;
};

using output = basic_output_iter;

#if BOOST_STL_INTERFACES_USE_CONCEPTS
static_assert(std::output_iterator<output, int>, "");
BOOST_STL_INTERFACES_STATIC_ASSERT_ITERATOR_TRAITS(
    output,
    void,
    std::output_iterator_tag,
    void,
    void,
    void,
    std::ptrdiff_t)
#else
BOOST_STL_INTERFACES_STATIC_ASSERT_ITERATOR_TRAITS(
    output,
    std::output_iterator_tag,
    std::output_iterator_tag,
    int,
    int &,
    void,
    std::ptrdiff_t)
#endif


template<typename Container>
struct back_insert_iter : boost::stl_interfaces::iterator_interface<
#if !BOOST_STL_INTERFACES_USE_DEDUCED_THIS
                              back_insert_iter<Container>,
#endif
                              std::output_iterator_tag,
                              typename Container::value_type,
                              back_insert_iter<Container> &>
{
    back_insert_iter() : c_(nullptr) {}
    back_insert_iter(Container & c) : c_(std::addressof(c)) {}

    back_insert_iter & operator*() noexcept { return *this; }
    back_insert_iter & operator++() noexcept { return *this; }

    back_insert_iter & operator=(typename Container::value_type const & v)
    {
        c_->push_back(v);
        return *this;
    }
    back_insert_iter & operator=(typename Container::value_type && v)
    {
        c_->push_back(std::move(v));
        return *this;
    }

    using base_type = boost::stl_interfaces::iterator_interface<
#if !BOOST_STL_INTERFACES_USE_DEDUCED_THIS
        back_insert_iter<Container>,
#endif
        std::output_iterator_tag,
        typename Container::value_type,
        back_insert_iter<Container> &>;
    using base_type::operator++;

private:
    Container * c_;
};

using back_insert = back_insert_iter<std::vector<int>>;

#if BOOST_STL_INTERFACES_USE_CONCEPTS
static_assert(std::output_iterator<back_insert, int>, "");
BOOST_STL_INTERFACES_STATIC_ASSERT_ITERATOR_TRAITS(
    back_insert,
    void,
    std::output_iterator_tag,
    void,
    void,
    void,
    std::ptrdiff_t)
#else
BOOST_STL_INTERFACES_STATIC_ASSERT_ITERATOR_TRAITS(
    back_insert,
    std::output_iterator_tag,
    std::output_iterator_tag,
    int,
    back_insert &,
    void,
    std::ptrdiff_t)
#endif


std::vector<int> ints = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};


int main()
{

{
    std::vector<int> ints_copy(ints.size());
    std::copy(ints.begin(), ints.end(), output(&ints_copy[0]));
    BOOST_TEST(ints_copy == ints);
}


{
    std::vector<int> ints_copy;
    std::copy(ints.begin(), ints.end(), back_insert(ints_copy));
    BOOST_TEST(ints_copy == ints);
}

    return boost::report_errors();
}

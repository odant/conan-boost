//  Copyright (c) 2012 Robert Ramey
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/core/demangle.hpp>

#include <boost/safe_numerics/checked_result.hpp>
#include <boost/safe_numerics/checked_result_operations.hpp>
#include <boost/safe_numerics/checked_integer.hpp>

// note: T should be of tyme checked_result<R> for some integer type R
template<class T>
bool test_checked_right_shift(
    T v1,
    T v2,
    char expected_result
){
    using namespace boost::safe_numerics;
    const T result = v1 >> v2;
    std::cout
        << v1 << " >> " << v2 << " -> " << result
        << std::endl;

    switch(expected_result){
    case '.':
        if(result.exception()){
            std::cout
                << "erroneously detected error in right shift "
                << std::endl;
            v1 >> v2;
            return false;
        }
        return true;
    case '-':
        if(safe_numerics_error::negative_overflow_error == result.m_e)
            return true;
        break;
    case '+':
        if(safe_numerics_error::positive_overflow_error == result.m_e)
            return true;
        break;
    case '!':
        if(safe_numerics_error::range_error == result.m_e)
            return true;
        break;
    case 'n':   // n negative_shift
        if(safe_numerics_error::negative_shift == result.m_e)
            return true;
        break;
    case 's':   // s negative_value_shift
        if(safe_numerics_error::negative_value_shift == result.m_e)
            return true;
        break;
    case 'l':   // l shift_too_large
        if(safe_numerics_error::shift_too_large == result.m_e)
            return true;
        break;
    default:
        assert(false);
    }
    std::cout
        << "failed to detect error in right shift "
        << std::hex << result << "(" << std::dec << result << ")"
        << " != "<< v1 << " >> " << v2
        << std::endl;
    v1 >> v2;
    return false;
}

#include "test_checked_right_shift.hpp"

template<typename T, typename First, typename Second>
struct test_signed_pair {
    bool operator()() const {
        std::size_t i = First();
        std::size_t j = Second();
        std::cout << std::dec << i << ',' << j << ','
            << "testing " << boost::core::demangle(typeid(T).name()) << ' ';
        return test_checked_right_shift(
            signed_values<T>[i],
            signed_values<T>[j],
            signed_right_shift_results[i][j]
        );
    };
};

template<typename T, typename First, typename Second>
struct test_unsigned_pair {
    bool operator()() const {
        std::size_t i = First();
        std::size_t j = Second();
        std::cout << std::dec << i << ',' << j << ','
            << "testing " << boost::core::demangle(typeid(T).name()) << ' ';
        return test_checked_right_shift(
            unsigned_values<T>[i],
            unsigned_values<T>[j],
            unsigned_right_shift_results[i][j]
        );
    };
};

#include <boost/mp11/algorithm.hpp>

int main(){
    using namespace boost::mp11;

    bool rval = true;

    std::cout << "*** testing signed values\n";

    mp_for_each<
        mp_product<
            test_signed_pair,
            signed_test_types,
            signed_value_indices,
            signed_value_indices
        >
    >([&](auto I){
        rval &= I();
    });

    std::cout << "*** testing unsigned values\n";

    mp_for_each<
        mp_product<
            test_unsigned_pair,
            unsigned_test_types,
            unsigned_value_indices, unsigned_value_indices
        >
    >([&](auto I){
        rval &= I();
    });

    std::cout << (rval ? "success!" : "failure") << std::endl;
    return rval ? 0 : 1;
}

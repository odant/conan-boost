// Copyright 2015-2018 Hans Dembinski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/core/lightweight_test.hpp>
#include <boost/histogram/accumulators/ostream.hpp>
#include <boost/histogram/accumulators/sum.hpp>
#include <boost/histogram/accumulators/weighted_sum.hpp>
#include <boost/histogram/detail/square.hpp>
#include <boost/histogram/weight.hpp>
#include <sstream>
#include "str.hpp"
#include "throw_exception.hpp"

using namespace boost::histogram;
using namespace std::literals;

using w_t = accumulators::weighted_sum<double>;

int main() {
  {
    w_t w;
    BOOST_TEST_EQ(str(w), "weighted_sum(0, 0)"s);
    BOOST_TEST_EQ(str(w, 20, false), "  weighted_sum(0, 0)"s);
    BOOST_TEST_EQ(str(w, 20, true), "weighted_sum(0, 0)  "s);
    BOOST_TEST_EQ(w, w_t{});

    BOOST_TEST_EQ(w, w_t(0));
    BOOST_TEST_NE(w, w_t(1));
    w = w_t(1);
    BOOST_TEST_EQ(w.value(), 1);
    BOOST_TEST_EQ(w.variance(), 1);
    BOOST_TEST_EQ(w, 1);
    BOOST_TEST_NE(w, 2);

    w += weight(2);
    BOOST_TEST_EQ(w.value(), 3);
    BOOST_TEST_EQ(w.variance(), 5);
    BOOST_TEST_EQ(w, w_t(3, 5));
    BOOST_TEST_NE(w, w_t(3));

    w += w_t(1, 2);
    BOOST_TEST_EQ(w.value(), 4);
    BOOST_TEST_EQ(w.variance(), 7);

    // consistency: a weighted counter increased by weight 1 multiplied
    // by 2 must be the same as a weighted counter increased by weight 2
    w_t u(0);
    ++u;
    u *= 2;
    BOOST_TEST_EQ(u, w_t(2, 4));

    w_t v(0);
    v += weight(2);
    BOOST_TEST_EQ(u, v);

    // conversion to RealType
    w_t y(1, 2);
    BOOST_TEST_NE(y, 1);
    BOOST_TEST_EQ(static_cast<double>(y), 1);

    BOOST_TEST_EQ(w_t() += w_t(), w_t());
  }

  // sum nested in weighted_sum
  {
    using s_t = accumulators::weighted_sum<accumulators::sum<double>>;
    s_t w;

    ++w;
    w += weight(1e100);
    ++w;
    w += weight(-1e100);

    BOOST_TEST_EQ(w.value(), 2);
    BOOST_TEST_EQ(w.variance(), 2e200);

    BOOST_TEST_EQ(s_t() += s_t(), s_t());
  }

  // division 1
  {
    w_t a{1, 2};
    w_t b{2, 3};

    auto c = a;
    c /= b;

    BOOST_TEST_EQ(c.value(), 0.5);
    // error propagation for independent a and b:
    // var(c)/c^2 = var(a)/a^2 + var(b)/b^2
    BOOST_TEST_EQ(c.variance() / detail::square(c.value()),
                  a.variance() / detail::square(a.value()) +
                      b.variance() / detail::square(b.value()));
  }

  // division with implicit conversion
  {
    w_t a{1, 2};
    double b = 2;

    auto c = a;
    c /= b;

    BOOST_TEST_EQ(c.value(), 0.5);
    // var(b) = b
    BOOST_TEST_EQ(c.variance() / detail::square(c.value()),
                  a.variance() / detail::square(a.value()) + 1.0 / b);
  }

  return boost::report_errors();
}

// Boost.Units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2008 Matthias Christian Schabel
// Copyright (C) 2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** 
\file
    
\brief test_absolute.cpp

\details
Test absolute units.

Output:
@verbatim
@endverbatim
**/

#include <boost/units/quantity.hpp>
#include <boost/units/absolute.hpp>
#include <boost/units/unit.hpp>
#include <boost/units/make_system.hpp>
#include <boost/units/physical_dimensions.hpp>
#include <boost/units/base_units/si/kelvin.hpp>
#include <boost/units/base_units/temperature/celsius.hpp>
#include <boost/units/base_units/temperature/fahrenheit.hpp>

#include <iostream>

#include <boost/core/lightweight_test.hpp>

#define BOOST_UNITS_CHECK_CLOSE(a, b) BOOST_TEST(std::abs((a) - (b)) < .0000001)

namespace bu = boost::units;
using bu::si::kelvin_base_unit;
using bu::temperature::celsius_base_unit;
using bu::temperature::fahrenheit_base_unit;

typedef bu::unit<bu::temperature_dimension,bu::make_system<kelvin_base_unit>::type> kelvin_type;

typedef bu::unit<bu::temperature_dimension,bu::make_system<celsius_base_unit>::type> celsius_type;

typedef bu::unit<bu::temperature_dimension,bu::make_system<fahrenheit_base_unit>::type> fahrenheit_type;

int main()
{
    BOOST_CONSTEXPR_OR_CONST bu::quantity<bu::absolute<fahrenheit_type> > q1(212.0 * bu::absolute<fahrenheit_type>());
    BOOST_CONSTEXPR_OR_CONST bu::quantity<bu::absolute<celsius_type> > q2(0.0 * bu::absolute<celsius_type>());
    BOOST_CONSTEXPR_OR_CONST bu::quantity<bu::absolute<fahrenheit_type> > q3(q2);
    BOOST_CONSTEXPR_OR_CONST bu::quantity<fahrenheit_type> q4(q1 - q3);

    BOOST_UNITS_CHECK_CLOSE(q4.value(), 180.0);

    BOOST_CONSTEXPR_OR_CONST bu::quantity<bu::absolute<kelvin_type> > q5(static_cast<bu::quantity<kelvin_type> >(q4) + static_cast<bu::quantity<bu::absolute<kelvin_type> > >(q2));

    BOOST_UNITS_CHECK_CLOSE(q5.value(), 373.15);

    BOOST_CONSTEXPR_OR_CONST bu::quantity<bu::absolute<fahrenheit_type> > q6(q5);

    BOOST_UNITS_CHECK_CLOSE(q6.value(), 212.0);

    return boost::report_errors();
}

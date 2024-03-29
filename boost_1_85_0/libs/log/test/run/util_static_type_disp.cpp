/*
 *          Copyright Andrey Semashev 2007 - 2015.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */
/*!
 * \file   util_static_type_disp.cpp
 * \author Andrey Semashev
 * \date   09.01.2009
 *
 * \brief  This header contains tests for the static type dispatcher.
 */

#define BOOST_TEST_MODULE util_static_type_disp

#include <string>
#include <typeinfo>
#include <boost/mpl/vector.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>
#include <boost/log/utility/type_dispatch/static_type_dispatcher.hpp>

namespace logging = boost::log;

namespace {

    // A simple attribute value
    template< typename T >
    struct my_value
    {
        T m_Value;

        explicit my_value(T const& value) : m_Value(value) {}

        // The function passes the contained type into the dispatcher
        bool dispatch(logging::type_dispatcher& dispatcher)
        {
            logging::type_dispatcher::callback< T > callback = dispatcher.get_callback< T >();
            if (callback)
            {
                callback(m_Value);
                return true;
            }
            else
                return false;
        }
    };

    // The function tests general functionality of the type dispatcher
    template< typename DispatcherT >
    void test_general_functionality(DispatcherT& disp)
    {
        // These two attributes are supported by the dispatcher
        my_value< std::string > val1("Hello world!");
        disp.set_expected(val1.m_Value);
        BOOST_CHECK(val1.dispatch(disp));

        my_value< double > val2(1.2);
        disp.set_expected(val2.m_Value);
        BOOST_CHECK(val2.dispatch(disp));

        // This one is not
        my_value< float > val3(static_cast< float >(-4.3));
        disp.set_expected();
        BOOST_CHECK(!val3.dispatch(disp));
    }


    // Type dispatcher for the supported types
    struct my_dispatcher :
        public logging::static_type_dispatcher<
            boost::mpl::vector< int, double, std::string >
        >
    {
        typedef logging::static_type_dispatcher<
            boost::mpl::vector< int, double, std::string >
        > base_type;

        enum type_expected
        {
            none_expected,
            int_expected,
            double_expected,
            string_expected
        };

        my_dispatcher() :
            base_type(*this),
            m_Expected(none_expected),
            m_Int(0),
            m_Double(0.0)
        {
        }

        void set_expected()
        {
            m_Expected = none_expected;
        }
        void set_expected(int value)
        {
            m_Expected = int_expected;
            m_Int = value;
        }
        void set_expected(double value)
        {
            m_Expected = double_expected;
            m_Double = value;
        }
        void set_expected(std::string const& value)
        {
            m_Expected = string_expected;
            m_String = value;
        }

        // Implement visitation logic for all supported types
        void operator() (int const& value) const
        {
            BOOST_CHECK_EQUAL(m_Expected, int_expected);
            BOOST_CHECK_EQUAL(m_Int, value);
        }
        void operator() (double const& value) const
        {
            BOOST_CHECK_EQUAL(m_Expected, double_expected);
            BOOST_CHECK_CLOSE(m_Double, value, 0.001);
        }
        void operator() (std::string const& value) const
        {
            BOOST_CHECK_EQUAL(m_Expected, string_expected);
            BOOST_CHECK_EQUAL(m_String, value);
        }

    private:
        type_expected m_Expected;
        int m_Int;
        double m_Double;
        std::string m_String;
    };

} // namespace

// The test checks that general functionality works
BOOST_AUTO_TEST_CASE(type_dispatch)
{
    my_dispatcher disp;
    test_general_functionality(disp);
}

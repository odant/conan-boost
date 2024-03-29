// (c) Copyright Juergen Hunold 2012
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE.txt or copy at
// https://www.bfgroup.xyz/b2/LICENSE.txt)

#define BOOST_TEST_MODULE QtGui

#include <QtWidgets/QApplication>

#include <boost/test/unit_test.hpp>

struct Fixture
{
    Fixture()
        : application(boost::unit_test::framework::master_test_suite().argc,
                      boost::unit_test::framework::master_test_suite().argv,
                      false)
    {
        BOOST_TEST_MESSAGE( "setup QApplication fixture" );
    }

    ~Fixture()
    {
        BOOST_TEST_MESSAGE( "teardown QApplication fixture" );
    }

    QApplication application;
};

BOOST_GLOBAL_FIXTURE( Fixture );

BOOST_AUTO_TEST_CASE( defines)
{
    BOOST_CHECK_EQUAL(BOOST_IS_DEFINED(QT_CORE_LIB), true);
    BOOST_CHECK_EQUAL(BOOST_IS_DEFINED(QT_GUI_LIB), true);
    BOOST_CHECK_EQUAL(BOOST_IS_DEFINED(QT_WIDGETS_LIB), true);
}


BOOST_AUTO_TEST_CASE( qtgui_test)
{
    BOOST_CHECK_EQUAL(true, true);
}

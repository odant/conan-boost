// (c) Copyright Juergen Hunold 2012
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE.txt or copy at
// https://www.bfgroup.xyz/b2/LICENSE.txt)

#include <QtTest>

class QtTest: public QObject
{
    /*!
      Test if the moc gets the #define
    */
#if defined(TEST_MOCK)
    Q_OBJECT
#endif

private Q_SLOTS:
    void toUpper();
};

void
QtTest::toUpper()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}

QTEST_MAIN(QtTest)
#include "qttest.moc"


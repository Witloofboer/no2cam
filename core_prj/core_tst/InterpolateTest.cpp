#include "InterpolateTest.h"
#include "Interpolate.cpp"

#include <QtTest>

using namespace core;

//------------------------------------------------------------------------------

void BiInterpolationTest::simpleTest()
{
    BiInterpolation interp(
    {0.0, 1.0},
    {0.0, 2.0},
    {   {1000.0, 1010.0},
        {1100.0, 1110.0}
    }); // interp(x, y) = 1000 + 100*x + 5*y

    QCOMPARE(interp(0.0, 0.0), 1000.0);
    QCOMPARE(interp(1.0, 0.0), 1100.0);
    QCOMPARE(interp(0.0, 2.0), 1010.0);
    QCOMPARE(interp(1.0, 2.0), 1110.0);
    QCOMPARE(interp(0.1, 0.3), 1011.5);
}

//------------------------------------------------------------------------------

void BiInterpolationTest::middleValuesTest()
{
    BiInterpolation interp(
    {0.0, 1.0, 3.0},
    {0.0, 1.0, 2.0},
    {   {  10.0,   20.0,   10.0},
        { 100.0,  200.0,  400.0},
        {1000.0, 2000.0,    0.0}
    });

    QCOMPARE(interp(0.0, 0.0),   10.0);
    QCOMPARE(interp(0.0, 0.1),   11.0);
    QCOMPARE(interp(0.0, 0.9),   19.0);
    QCOMPARE(interp(0.0, 1.0),   20.0);
    QCOMPARE(interp(0.0, 1.1),   19.0);
    QCOMPARE(interp(0.0, 1.9),   11.0);
    QCOMPARE(interp(0.0, 2.0),   10.0);

    QCOMPARE(interp(0.1, 0.0),   19.0);
    QCOMPARE(interp(0.5, 0.0),   55.0);
    QCOMPARE(interp(0.9, 0.0),   91.0);
    QCOMPARE(interp(1.0, 0.0),  100.0);

    QCOMPARE(interp(1.2, 0.0),   190.0);
    QCOMPARE(interp(2.0, 0.0),   550.0);
    QCOMPARE(interp(2.8, 0.0),   910.0);

    QCOMPARE(interp(0.5, 0.5),   82.5);
    QCOMPARE(interp(0.5, 1.5),  157.5);
    QCOMPARE(interp(2.0, 0.5),  825.0);
    QCOMPARE(interp(2.0, 1.5),  650.0);
}

//------------------------------------------------------------------------------

void BiInterpolationTest::asymetric1Test()
{
    BiInterpolation interp(
    {0.0, 1.0},
    {0.0, 1.0, 2.0},
    {   {  10.0,   20.0,   10.0},
        { 100.0,  200.0,  400.0}
    });

    QCOMPARE(interp(0.0, 0.0),   10.0);
    QCOMPARE(interp(0.0, 0.1),   11.0);
    QCOMPARE(interp(0.0, 0.9),   19.0);
    QCOMPARE(interp(0.0, 1.0),   20.0);
    QCOMPARE(interp(0.0, 1.1),   19.0);
    QCOMPARE(interp(0.0, 1.9),   11.0);
    QCOMPARE(interp(0.0, 2.0),   10.0);

    QCOMPARE(interp(0.1, 0.0),   19.0);
    QCOMPARE(interp(0.5, 0.0),   55.0);
    QCOMPARE(interp(0.9, 0.0),   91.0);
    QCOMPARE(interp(1.0, 0.0),  100.0);

    QCOMPARE(interp(0.5, 0.5),   82.5);
    QCOMPARE(interp(0.5, 1.5),  157.5);
}

//------------------------------------------------------------------------------

void BiInterpolationTest::asymetric2Test()
{
    BiInterpolation interp(
    {0.0, 1.0, 3.0},
    {0.0, 1.0},
    {   {  10.0,   20.0},
        { 100.0,  200.0},
        {1000.0, 2000.0}
    });

    QCOMPARE(interp(0.0, 0.0),   10.0);
    QCOMPARE(interp(0.0, 0.1),   11.0);
    QCOMPARE(interp(0.0, 0.9),   19.0);
    QCOMPARE(interp(0.0, 1.0),   20.0);

    QCOMPARE(interp(0.1, 0.0),   19.0);
    QCOMPARE(interp(0.5, 0.0),   55.0);
    QCOMPARE(interp(0.9, 0.0),   91.0);
    QCOMPARE(interp(1.0, 0.0),  100.0);

    QCOMPARE(interp(0.5, 0.5),   82.5);
}

//------------------------------------------------------------------------------

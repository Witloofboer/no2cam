#include <cmath>
#include <QtTest>

#include "../core/AotfCrystal.h"

#include "AotfCrystalTest.h"
#include "TestHarness.h"

double f(double x) {return 1e-3*round(x*1e3);}

void AotfCrystalTest::acousticParam()
{
    QFETCH(double, lambda);
    QFETCH(double, T);
    QFETCH(double, alpha);
    QFETCH(double, theta);
    QFETCH(double, Ht);
    QFETCH(double, Lt);
    QFETCH(double, freq);
    QFETCH(double, power);

    AotfCrystal crystal;
    crystal.parameters(alpha, theta, Ht, Lt);

    QCOMPARE(f(crystal.acousticFreq(lambda, T)), f(freq));
    QCOMPARE(f(crystal.acousticPower(lambda, T)), f(power));
}

void AotfCrystalTest::acousticParam_data()
{
    QTest::addColumn<double>("lambda");
    QTest::addColumn<double>("T");
    QTest::addColumn<double>("alpha");
    QTest::addColumn<double>("theta");
    QTest::addColumn<double>("Ht");
    QTest::addColumn<double>("Lt");
    QTest::addColumn<double>("freq");
    QTest::addColumn<double>("power");

    QTest::newRow("Ref")    << 450.0 << 21.0 << 7.5 << 10.0 << 10.0 << 10.0 << 142.123 << 146.268;
    QTest::newRow("Blue")   << 500.0 << 21.0 << 7.5 << 10.0 << 10.0 << 10.0 << 121.882 << 201.065;
    QTest::newRow("Hot")    << 450.0 << 31.0 << 7.5 << 10.0 << 10.0 << 10.0 << 142.456 << 146.874;
    QTest::newRow("Alpha")  << 450.0 << 21.0 << 8.0 << 10.0 << 10.0 << 10.0 << 152.795 << 154.258;
    QTest::newRow("Theta")  << 450.0 << 21.0 << 7.5 << 11.0 << 10.0 << 10.0 << 142.559 << 151.870;
    QTest::newRow("t_high") << 450.0 << 21.0 << 7.5 << 10.0 << 11.0 << 10.0 << 142.123 << 160.895;
    QTest::newRow("t_wide") << 450.0 << 21.0 << 7.5 << 10.0 << 10.0 << 11.0 << 142.123 << 132.971;
}

static AotfCrystalTest testCase;

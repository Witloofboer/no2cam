#include <cmath>
#include <QtTest>

#include "../core/Crystal.h"

#include "AotfCrystalTest.h"

using namespace core;

double f(double x) {return 1e-3*round(x*1e3);}

void AotfCrystalTest::initTestCase()
{
}

void AotfCrystalTest::cleanupTestCase()
{
}

void AotfCrystalTest::frequency_power()
{
    QFETCH(double, lambda);
    QFETCH(double, T);
    QFETCH(double, alpha);
    QFETCH(double, theta);
    QFETCH(double, Ht);
    QFETCH(double, Lt);
    QFETCH(double, freq);
    QFETCH(double, power);

    Crystal crystal;
    crystal.setParameters({alpha, theta, Ht, Lt});

    QCOMPARE(f(crystal.frequency(lambda, T)), f(freq));
    QCOMPARE(f(crystal.power(lambda, T)), f(power));
}

void AotfCrystalTest::frequency_power_data()
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

void AotfCrystalTest::wavelength()
{
    QFETCH(double, lambda);
    QFETCH(double, T);

    Crystal crystal;
    crystal.setParameters({7.65, 10.1, 10.0, 15.0});

    const double freq = crystal.frequency(lambda, T);

    QCOMPARE(f(crystal.wavelength(freq, T)), f(lambda));
}

void AotfCrystalTest::wavelength_data()
{
    QTest::addColumn<double>("lambda");
    QTest::addColumn<double>("T");

    QTest::newRow("0") << 460.0 << 21.0;
    QTest::newRow("1") << 470.0 << 21.0;
    QTest::newRow("2") << 460.0 << 31.0;
    QTest::newRow("3") << 470.0 << 31.0;
}

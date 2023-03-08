#ifndef INTERPOLATIONTEST_H
#define INTERPOLATIONTEST_H

#include <QObject>

class BiInterpolatorTest : public QObject
{
    Q_OBJECT

private slots:
    void simpleTest();
    void interpolationTest();
    void extrapolationTest();
    void asymetric1Test();
    void asymetric2Test();
    void constructorExceptionsTest();
};

#endif // INTERPOLATIONTEST_H

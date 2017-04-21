#ifndef INTERPOLATIONTEST_H
#define INTERPOLATIONTEST_H

#include <QObject>

class BiInterpolationTest : public QObject
{
    Q_OBJECT

private slots:
    void simpleTest();
    void middleValuesTest();
    void asymetric1Test();
    void asymetric2Test();
    void constructorExceptionsTest();
    void interpolationExceptionsTest();
};

#endif // INTERPOLATIONTEST_H

#ifndef AOTFCRYSTALTEST_H
#define AOTFCRYSTALTEST_H

#include "TestCase.h"

class AotfCrystalTest : public TestCase
{
    Q_OBJECT

private slots:
    void frequency_power();
    void frequency_power_data();
    void wavelength();
    void wavelength_data();
};

#endif // AOTFCRYSTALTEST_H

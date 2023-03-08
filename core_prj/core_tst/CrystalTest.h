#ifndef AOTFCRYSTALTEST_H
#define AOTFCRYSTALTEST_H

#include <QObject>

//------------------------------------------------------------------------------

class AotfCrystalTest : public QObject
{
    Q_OBJECT

private slots:
    void frequency_power();
    void frequency_power_data();
    void wavelength();
    void wavelength_data();
};

//------------------------------------------------------------------------------

#endif // AOTFCRYSTALTEST_H

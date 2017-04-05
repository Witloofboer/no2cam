#ifndef DRIVER_H
#define DRIVER_H
#include "core/drivers.h"

#include <QtGlobal>

class QSerialPort;

class BaseDriver : public core::AcousticDriver
{
    Q_OBJECT

public:
    BaseDriver();
    bool init();

    static BaseDriver *getDriver();

protected:
    QSerialPort *_serial;

};

//------------------------------------------------------------------------------

class DdsDriver : public BaseDriver
{
    Q_OBJECT

public:
    DdsDriver();

    void set(double frequency, double power) override;
};

//------------------------------------------------------------------------------

class PllDriver : public BaseDriver
{
    Q_OBJECT

public:
    PllDriver();

    void set(double frequency, double power) override;
};

//------------------------------------------------------------------------------

#endif // DRIVER_H

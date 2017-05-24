#ifndef DRIVER_H
#define DRIVER_H
#include "core/BaseDrivers.h"

#include <QtGlobal>
#include "core/Interpolation.h"

//------------------------------------------------------------------------------

class QSerialPort;
class QSerialPortInfo;

//------------------------------------------------------------------------------

class BaseDriver : public core::BaseAcousticDriver
{
    Q_OBJECT

public:
    BaseDriver(QSerialPort *serial);

    static BaseDriver *getDriver();

protected:
    QSerialPort *_serial;

private:
    bool _ok;

};

//------------------------------------------------------------------------------

class DdsDriver : public BaseDriver
{
    Q_OBJECT

public:
    DdsDriver(QSerialPort *serial);

    void set(double frequency, double power) override;

private:
    QByteArray _stream;
    const core::BiInterpolator _interpolator;
};

//------------------------------------------------------------------------------

class PllDriver : public BaseDriver
{
    Q_OBJECT

public:
    PllDriver(QSerialPort *serial);

    void set(double frequency, double power) override;
};

//------------------------------------------------------------------------------

#endif // DRIVER_H

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

protected:
    BaseDriver(QSerialPort *serial);
    QSerialPort *_serial;

private:
    bool _ok;

};

//------------------------------------------------------------------------------

class DdsDriver : public BaseDriver
{
    Q_OBJECT

public:
    static DdsDriver *getDriver();

    void set(double frequency, double power) override;

private:
    DdsDriver(QSerialPort *serial);

    QByteArray _stream;
    const core::BiInterpolator _interpolator;
};

//------------------------------------------------------------------------------

class PllDriver : public BaseDriver
{
    Q_OBJECT

public:
    static PllDriver *getDriver();

    void set(double frequency, double power) override;

private:
    PllDriver(QSerialPort *serial);
};

//------------------------------------------------------------------------------

#endif // DRIVER_H

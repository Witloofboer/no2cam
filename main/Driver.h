#ifndef DRIVER_H
#define DRIVER_H
#include "core/drivers.h"

#include <QtGlobal>

class QSerialPort;

class Driver : public core::AcousticDriver
{
    Q_OBJECT

public:
    Driver();

    bool init();
    void uninit();

    void set(double frequency, double power) override;

private:
    void writeDDS(double frequency, quint8 level);
    void writePLL(double frequency);

private:
    QSerialPort *_serial;

};

#endif // DRIVER_H

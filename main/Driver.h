#ifndef DRIVER_H
#define DRIVER_H
#include "core/drivers.h"

class QSerialPort;

class Driver : public core::AcousticDriver
{
    Q_OBJECT

public:
    Driver();

    bool init();
    void uninit();

    void set(double frequency, double power) override;
    void writeDDS(double frequency, double power);
    void writePLL(double frequency);

public slots:
    void serialReceived();

private:
    QSerialPort *_serial;
};

#endif // DRIVER_H

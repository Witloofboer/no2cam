#ifndef DRIVER_H
#define DRIVER_H
#include "core/drivers.h"

class Generator;
class QSerialPort;

class Driver : public core::PowerDriver
{
    Q_OBJECT

public:
    Driver(const Generator* generator);

    void setPower(double power) override;
    void writeDDS(double power);
    void writePLL();

public slots:
    void serialReceived();

private:
    QSerialPort *_serial;
    const Generator* _generator;
};

#endif // DRIVER_H

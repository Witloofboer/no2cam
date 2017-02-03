#ifndef DRIVER_H
#define DRIVER_H
#include "core/AbstractGenerator.h"
#include "core/AbstractDriver.h"
#include <string.h>

class Generator;
class QSerialPort;

class Driver : public core::AbstractDriver
{
    Q_OBJECT

public:
    Driver(const Generator* generator);

    virtual void stop() override;
    virtual void setPower(double power) override;
    void writeDDS(double power);
    void writePLL();

public slots:
    void serialReceived();

private:
    QSerialPort *_serial;
    const Generator* _generator;
};

#endif // DRIVER_H

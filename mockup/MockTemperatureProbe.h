#ifndef MOCKCRYSTEMPPROBE_H
#define MOCKCRYSTEMPPROBE_H

#include "core/AbstractCrysTempProbe.h"

//------------------------------------------------------------------------------

class MockTemperatureProbe : public core::AbstractCrysTempProbe
{
public:
    MockTemperatureProbe();
    virtual double getTemperature() override;

private:
    double _temperature;
    double _delta;
};

//------------------------------------------------------------------------------

#endif // MOCKCRYSTEMPPROBE_H

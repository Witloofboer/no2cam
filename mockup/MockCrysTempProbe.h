#ifndef MOCKCRYSTEMPPROBE_H
#define MOCKCRYSTEMPPROBE_H

#include "core/AbstractCrysTempProbe.h"

//------------------------------------------------------------------------------

class MockCrysTempProbe : public core::AbstractCrysTempProbe
{
public:
    MockCrysTempProbe();
    virtual double getTemperature() override;
};

//------------------------------------------------------------------------------

#endif // MOCKCRYSTEMPPROBE_H

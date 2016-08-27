#ifndef MOCKDRIVER_H
#define MOCKDRIVER_H

//------------------------------------------------------------------------------

#include "core/AbstractDriver.h"

//------------------------------------------------------------------------------

class MockDriver : public core::AbstractDriver
{
    Q_OBJECT

public:
    MockDriver();

    virtual void stop() override;
    virtual void setPower(double power) override;
    virtual void cooldown() override;

};

//------------------------------------------------------------------------------

#endif // MOCKDRIVER_H

#ifndef MOCKDRIVER_H
#define MOCKDRIVER_H

//------------------------------------------------------------------------------

#include "core/Driver.h"

//------------------------------------------------------------------------------

class MockDriver : public core::AbstractDriver
{
    Q_OBJECT

public:
    MockDriver();

    virtual void setPower(double power) override;
};

//------------------------------------------------------------------------------

#endif // MOCKDRIVER_H

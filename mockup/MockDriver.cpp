#include "MockDriver.h"

#include <QCoreApplication>

//------------------------------------------------------------------------------

MockDriver::MockDriver()
    : core::AbstractDriver()
{
}

//------------------------------------------------------------------------------

void MockDriver::stop()
{
    setPower(0.0);
}

//------------------------------------------------------------------------------

void MockDriver::setPower(double power)
{
    qInfo("Driver: set power: %.1f mW", power);
}

//------------------------------------------------------------------------------

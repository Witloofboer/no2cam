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
    qInfo("Driver: stop");
}

//------------------------------------------------------------------------------

void MockDriver::setPower(double power)
{
    qInfo("Driver: set power: %.1f mW", power);
}

//------------------------------------------------------------------------------

void MockDriver::cooldown()
{
    qInfo("Driver: cooldown");
}

//------------------------------------------------------------------------------

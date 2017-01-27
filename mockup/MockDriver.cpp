#include "MockDriver.h"

#include <QCoreApplication>

//------------------------------------------------------------------------------

MockDriver::MockDriver()
    : core::AbstractDriver()
{
}

//------------------------------------------------------------------------------

void MockDriver::setPower(double power)
{
    qDebug("Driver: power set to %.1f mW", power);
}

//------------------------------------------------------------------------------

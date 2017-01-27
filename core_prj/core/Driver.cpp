#include "Driver.h"

namespace core
{

//------------------------------------------------------------------------------

AbstractDriver::AbstractDriver()
    : QObject()
{}

//------------------------------------------------------------------------------

DriverProxy::DriverProxy(AbstractDriver *driver)
    : _driver(driver)
    , _power(-1.0)
{}

//------------------------------------------------------------------------------

bool DriverProxy::setPower(double power)
{
    if (_power == power)
    {
        return false;
    } else {
        _power = power;
        _driver->setPower(power);
        return true;
    }
}

//------------------------------------------------------------------------------

}

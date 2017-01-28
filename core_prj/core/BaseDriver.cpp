#include "BaseDriver.h"

namespace core
{

//------------------------------------------------------------------------------

BaseDriver::BaseDriver()
    : QObject()
    , _power(-1.0)
{}

//------------------------------------------------------------------------------

bool BaseDriver::setPower(double power)
{
    bool isUpdate = _power != power;

    if (isUpdate)
    {
        qDebug("Setting acoustic power to %.1f mW", power);
        _power = power;
        updatePower();
    }

    return isUpdate;
}

//------------------------------------------------------------------------------

MockDriver::MockDriver() : BaseDriver() {}

//------------------------------------------------------------------------------

void MockDriver::updatePower() {}

//------------------------------------------------------------------------------

}

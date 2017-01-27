#include "MockTemperatureProbe.h"

#include <math.h>

//------------------------------------------------------------------------------

MockTemperatureProbe::MockTemperatureProbe()
    : core::AbstractCrysTempProbe()
    , _temperature(20.0)
    , _delta(0.123456)
{

}

//------------------------------------------------------------------------------

double MockTemperatureProbe::getTemperature()
{
    _temperature += _delta;
    if (_temperature < 18.0) _delta = fabs(_delta);
    else if (22.0 < _temperature) _delta = -fabs(_delta);

    qDebug("Temperature probe: %f degC", _temperature);
    return _temperature;
}
//------------------------------------------------------------------------------

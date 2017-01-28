#include "BaseTemperatureProbe.h"

#include <limits>

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

BaseTemperatureProbe::BaseTemperatureProbe()
    : QObject()
    , _temperature(std::numeric_limits<double>::min()) // fake value
{}

//------------------------------------------------------------------------------

double BaseTemperatureProbe::getTemperature()
{
    readProbe();
    qDebug("Temperature is %f degC", _temperature);
    return _temperature;
}

//------------------------------------------------------------------------------

MockTemperatureProbe::MockTemperatureProbe()
    : core::BaseTemperatureProbe()
    , _delta(0.12)
{
    _temperature = 20.0;
    readProbe();
}

//------------------------------------------------------------------------------

void MockTemperatureProbe::readProbe()
{
    _temperature += _delta;
    if (_temperature < 18.0) _delta = fabs(_delta);
    else if (22.0 < _temperature) _delta = -fabs(_delta);
}

//------------------------------------------------------------------------------

}

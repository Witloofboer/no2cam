#include "CrysTempProbe.h"

//------------------------------------------------------------------------------

MockCrysTempProbe::MockCrysTempProbe()
    : core::AbstractCrysTempProbe()
{

}

//------------------------------------------------------------------------------

double MockCrysTempProbe::getTemperature()
{
    //tsp->getTemperatureData(2, 0, temperature);

    //return temperature;
    return 25;
}
//------------------------------------------------------------------------------

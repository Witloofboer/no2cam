#include "MockCrysTempProbe.h"

//------------------------------------------------------------------------------

MockCrysTempProbe::MockCrysTempProbe()
    : core::AbstractCrysTempProbe()
{

}

//------------------------------------------------------------------------------

double MockCrysTempProbe::getTemperature()
{
    return 25;
}
//------------------------------------------------------------------------------

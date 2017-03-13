#include "Thermometer.h"

#include <QMessageBox>

//--------------------------------------------------------------------------------------------//

Thermometer::Thermometer()
    : core::ThermometerDriver()
{}

//--------------------------------------------------------------------------------------------//

Thermometer::~Thermometer()
{
    TLTSP_close(_instrHdl);
}

//--------------------------------------------------------------------------------------------//

bool Thermometer::init()
{
    ViStatus      status;
    static ViChar rscStr[VI_FIND_BUFLEN];

    status = TLTSP_getDeviceResourceString(VI_NULL, 0, rscStr);
    if (status != VI_SUCCESS)
        return false;

    status = TLTSP_init(_rscPtr, VI_ON, VI_ON, &_instrHdl);
    if (status != VI_SUCCESS)
        return false;

    viSetAttribute(_instrHdl, VI_ATTR_TMO_VALUE, 5000);
    return true;
}

//------------------------------------------------------------------------------

double Thermometer::getTemperature() {
    ViReal64 temperature_value;
    _err = TLTSP_measTemperature (_instrHdl, TLTSP_MEAS_TEMP2, &temperature_value);
    return temperature_value;

}

//------------------------------------------------------------------------------

double Thermometer::getHumidity() {
    ViReal64 humidity_value;
    _err = TLTSP_measHumidity(_instrHdl, &humidity_value);
    return humidity_value;
}

//------------------------------------------------------------------------------

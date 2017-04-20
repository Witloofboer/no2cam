#include "Thermometer.h"

#include <QMessageBox>

//------------------------------------------------------------------------------

Thermometer::Thermometer(const ViSession &session)
    : core::BaseThermometerDriver()
    , _session(session)
{}

//------------------------------------------------------------------------------

Thermometer::~Thermometer()
{
    TLTSP_close(_session);
}

//------------------------------------------------------------------------------

Thermometer* Thermometer::getThermometer()
{
    ViStatus status;
    ViUInt32 findCnt;

    static ViChar rscStr[VI_FIND_BUFLEN];

    status = TLTSP_getDeviceCount((ViSession) VI_NULL, &findCnt);
    status = TLTSP_getDeviceResourceString(VI_NULL, 0, rscStr);

    if (status != VI_SUCCESS)
    {
        QMessageBox::critical(0, "Aborting",
                              "Failed to detect temperature sensor.");

        return nullptr;
    }

    ViSession session;

    status = TLTSP_init(rscStr, VI_ON, VI_ON, &session);
    if (status != VI_SUCCESS)
    {
        QMessageBox::critical(0, "Aborting",
                              "Failed to init temperature sensor.");
        return nullptr;
    }

    viSetAttribute(session, VI_ATTR_TMO_VALUE, 5000);
    return new Thermometer(session);
}

//------------------------------------------------------------------------------

double Thermometer::getTemperature() {
    ViReal64 temperature_value;
    TLTSP_measTemperature (_session, TLTSP_MEAS_TEMP2, &temperature_value);
    return temperature_value;

}

//------------------------------------------------------------------------------

double Thermometer::getHumidity() {
    ViReal64 humidity_value;
    TLTSP_measHumidity(_session, &humidity_value);
    return humidity_value;
}

//------------------------------------------------------------------------------

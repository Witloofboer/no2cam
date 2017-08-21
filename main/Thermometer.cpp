#include "Thermometer.h"

#include "core/Mockups.h"

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

core::BaseThermometerDriver* Thermometer::getThermometer()
{
    ViStatus status;
    ViUInt32 findCnt;

    static ViChar rscStr[VI_FIND_BUFLEN];

    status = TLTSP_getDeviceCount((ViSession) VI_NULL, &findCnt);
    status = TLTSP_getDeviceResourceString(VI_NULL, 0, rscStr);

    if (status != VI_SUCCESS)
    {
        qCritical("No temperature sensor detected");
        QMessageBox::critical(0,
                              "Aborting",
                              "<p><b>Failure</b>: no temperature sensor detected</p>"
                              "<p><b>Using the mockup sensor at 20°C</b></p>");

        return new core::MockThermometer(0.0);
    }

    ViSession session;

    status = TLTSP_init(rscStr, VI_ON, VI_ON, &session);
    if (status != VI_SUCCESS)
    {
        qCritical("Failure to initialise the temperature sensor");
        QMessageBox::critical(0,
                              "Aborting",
                              "<p>Failed to init temperature sensor</p>"
                              "<p><b>Using the mockup sensor at 20°C</b></p>");

        return new core::MockThermometer(0.0);
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

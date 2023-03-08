#ifndef THERMOMETER_H
#define THERMOMETER_H

#include "visa.h"
#include "TLTSP.h"
#include "core/BaseDrivers.h"

class Thermometer : public core::BaseThermometerDriver
{
    Q_OBJECT

public:
    Thermometer(const ViSession &session);
    virtual ~Thermometer() override;

    static BaseThermometerDriver *getThermometer();

    double getTemperature() override;
    double getHumidity();

private:
    ViStatus  find_instruments(ViString findPattern, ViChar **resource);
    ViSession _session;
};





#endif // THERMOMETER_H

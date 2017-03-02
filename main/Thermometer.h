#ifndef THERMOMETER_H
#define THERMOMETER_H

#include "visa.h"
#include "TLTSP.h"
#include "core/drivers.h"

class Thermometer : public core::ThermometerDriver
{
    Q_OBJECT

public:
    Thermometer();

    double getTemperature() override;
    double getHumidity();

    bool init();
    void uninit();

private:

    ViStatus    find_instruments(ViString findPattern, ViChar **resource);
    ViStatus    err;
    ViChar      *rscPtr;
    ViSession   instrHdl = VI_NULL;

};





#endif // THERMOMETER_H

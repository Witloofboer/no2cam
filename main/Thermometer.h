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
    virtual ~Thermometer() override;

    double getTemperature() override;
    double getHumidity();

    bool init();

private:

    ViStatus    find_instruments(ViString findPattern, ViChar **resource);
    ViStatus    _err;
    ViChar      *_rscPtr;
    ViSession   _instrHdl = VI_NULL;

};





#endif // THERMOMETER_H

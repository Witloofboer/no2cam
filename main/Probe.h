#ifndef PROBE_H
#define PROBE_H

#include "visa.h"
#include "TLTSP.h"
#include "core/drivers.h"

class Probe : public core::ProbeDriver
{
    Q_OBJECT

public:
    Probe();

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





#endif // PROBE_H

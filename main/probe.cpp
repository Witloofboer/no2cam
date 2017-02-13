#include "Probe.h"

ViStatus Probe::find_instruments(ViString findPattern, ViChar **resource)
{
    ViStatus       err;
    ViUInt32       findCnt;
    static ViChar  rscStr[VI_FIND_BUFLEN];

    //err = TLTSP_getDeviceCount(VI_NULL, );
    err = TLTSP_getDeviceCount((ViSession) VI_NULL,&findCnt);

    err = TLTSP_getDeviceResourceString(VI_NULL, 0, rscStr);
    *resource = rscStr;
    return (err);

}



void Probe::init()
{
    err = find_instruments(TLTSP_FIND_PATTERN_01, &rscPtr);

    err = TLTSP_init(rscPtr, VI_ON, VI_ON, &instrHdl);

    viSetAttribute(instrHdl, VI_ATTR_TMO_VALUE, 5000);
}

void Probe::uninit()
{
    TLTSP_close(instrHdl);
}



double Probe::getTemperature() {
    ViReal64 temperature_value;

    err = TLTSP_measTemperature (instrHdl, TLTSP_MEAS_TEMP2, &temperature_value);
    qDebug("temperature Value= %f ",temperature_value);
    return temperature_value;
}


Probe::Probe()
    : core::ProbeDriver()
{}

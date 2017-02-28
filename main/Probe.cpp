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

//--------------------------------------------------------------------------------------------//

bool Probe::init()
{
        err = find_instruments(TLTSP_FIND_PATTERN_01, &rscPtr);
        if (err != VI_SUCCESS)
        {
            //qDebug("Error on finding the temperature Probe");
            return false;
        }

        err = TLTSP_init(rscPtr, VI_ON, VI_ON, &instrHdl);
        if (err != VI_SUCCESS)
        {
           // qDebug("Error on initializing the temperature Probe");
            return false;
        }

        viSetAttribute(instrHdl, VI_ATTR_TMO_VALUE, 5000);
        return true;
        }

//--------------------------------------------------------------------------------------------//

void Probe::uninit()
{
    TLTSP_close(instrHdl);
}

//--------------------------------------------------------------------------------------------//

double Probe::getTemperature() {
   ViReal64 temperature_value;
   err = TLTSP_measTemperature (instrHdl, TLTSP_MEAS_TEMP2, &temperature_value);
   return temperature_value;

}

double Probe::getHumidity() {
    ViReal64 humidity_value;
    err = TLTSP_measHumidity(instrHdl, &humidity_value);
    return humidity_value;
}


Probe::Probe()
    : core::ProbeDriver()
{}

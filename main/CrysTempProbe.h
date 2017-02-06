#ifndef MOCKCRYSTEMPPROBE_H
#define MOCKCRYSTEMPPROBE_H

#include "core/AbstractCrysTempProbe.h"

//------------------------------------------------------------------------------

class MockCrysTempProbe : public core::AbstractCrysTempProbe
{
public:
    MockCrysTempProbe();
    virtual double getTemperature() override;

  // to adapt----
    //using namespace Thorlabs::TSP;
    //double temperature;

    //TLSP^ tsp = gcnew TLSP("USB0::0x1313::0x80F8::DEVICE-SERIAL-NUMBER::INSTR", true, true);
  //-------------
};

//------------------------------------------------------------------------------

#endif // MOCKCRYSTEMPPROBE_H

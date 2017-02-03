#include "Generator.h"

#include <QCoreApplication>

//------------------------------------------------------------------------------

Generator::Generator()
    : core::FrequencyDriver()
{

}

//------------------------------------------------------------------------------

void Generator::setFrequency(double frequency)
{
    qInfo("<frequency: %.3f MHz>", frequency);
    _frequency = frequency;
}

//------------------------------------------------------------------------------

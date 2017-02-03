#include "Generator.h"

#include <QCoreApplication>

//------------------------------------------------------------------------------

Generator::Generator()
    : core::AbstractGenerator()
{

}

//------------------------------------------------------------------------------

void Generator::stop()
{
    //setFrequency(0.0);
}

//------------------------------------------------------------------------------

void Generator::setFrequency(double frequency)
{
    qInfo("Generator: set frequency: %.3f MHz", frequency);
    _frequency = frequency;
}

//------------------------------------------------------------------------------

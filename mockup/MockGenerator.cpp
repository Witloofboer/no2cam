#include "MockGenerator.h"

#include <QCoreApplication>

//------------------------------------------------------------------------------

MockGenerator::MockGenerator()
    : core::AbstractGenerator()
{

}


//------------------------------------------------------------------------------

void MockGenerator::stop()
{
    qInfo("Generator: stop");
}


//------------------------------------------------------------------------------

void MockGenerator::setFrequency(double frequency)
{
    qInfo("Generator: set frequency: %.3f MHz", frequency);
}

//------------------------------------------------------------------------------

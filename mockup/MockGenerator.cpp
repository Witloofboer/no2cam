#include "MockGenerator.h"

#include <QCoreApplication>

//------------------------------------------------------------------------------

MockGenerator::MockGenerator()
    : core::AbstractGenerator()
{

}

//------------------------------------------------------------------------------

void MockGenerator::setFrequency(double frequency)
{
    qDebug("Generator: frequency set to %.3f MHz", frequency);
}

//------------------------------------------------------------------------------

#include "BaseGenerator.h"

namespace core
{

//------------------------------------------------------------------------------

BaseGenerator::BaseGenerator()
    : QObject()
    , _frequency(-1)
{}

//------------------------------------------------------------------------------

bool BaseGenerator::setFrequency(double frequency)
{
    bool isUpdate = _frequency != frequency;

    if (isUpdate)
    {
        qDebug("Setting acoustic frequency to %.3f MHz", frequency);
        _frequency = frequency;
        updateFrequency();
    }

    return isUpdate;
}

//------------------------------------------------------------------------------

MockGenerator::MockGenerator(): BaseGenerator() {}

//------------------------------------------------------------------------------

void MockGenerator::updateFrequency() {}

//------------------------------------------------------------------------------

}

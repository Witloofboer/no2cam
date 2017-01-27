#include "Generator.h"

namespace core
{

//------------------------------------------------------------------------------

AbstractGenerator::AbstractGenerator()
    : QObject()
{}

//------------------------------------------------------------------------------

GeneratorProxy::GeneratorProxy(AbstractGenerator *generator)
    : _generator(generator)
    , _frequency(-1)
{}

//------------------------------------------------------------------------------

bool GeneratorProxy::setFrequency(double frequency)
{
    if (_frequency == frequency)
    {
        return false;
    } else {
        _frequency = frequency;
        _generator->setFrequency(frequency);
        return true;
    }
}

//------------------------------------------------------------------------------

}

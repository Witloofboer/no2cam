#include "AbstractCamera.h"

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

AbstractCamera::AbstractCamera()
    : QObject()
    , _exposure(-1)
{}

void AbstractCamera::setExposure(int exposure)
{

    qInfo("Camera: exposure time set to %d ms", exposure);
    _exposure = exposure;
}

//------------------------------------------------------------------------------

}
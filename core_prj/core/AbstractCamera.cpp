#include "AbstractCamera.h"


core::AbstractCamera::AbstractCamera()
    : QObject()
    , _exposure(-1)
{}

void core::AbstractCamera::setExposure(int exposure)
{

    qInfo("Camera: exposure time set to %d ms", exposure);
    _exposure = exposure;
}

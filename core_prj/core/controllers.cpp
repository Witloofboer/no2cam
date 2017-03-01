#include "controllers.h"
#include "drivers.h"

#include <QDir> // Q_INIT_RESOURCE

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

AcousticCtrl::AcousticCtrl(QObject *parent, AcousticDriver *driver)
    : QObject(parent)
    , _driver(driver)
    , _frequency(0)
    , _power(0.0)
{
    _driver->setParent(this);
}

//------------------------------------------------------------------------------

bool AcousticCtrl::set(double frequency, double power)
{
    bool isUpdated = (_frequency != frequency) || (_power != power);

    if (isUpdated)
    {
        qDebug("Setting acoustic beam to %.3f MHz and %.1f mW", frequency, power);
        _frequency = frequency;
        _power = power;
        _driver->set(_frequency, _power);
    }

    return isUpdated;
}

//------------------------------------------------------------------------------

ProbeCtrl::ProbeCtrl(QObject *parent, ProbeDriver *probe)
    : QObject(parent)
    , _probe(probe)
{
    _probe->setParent(this);
}

//------------------------------------------------------------------------------

double ProbeCtrl::getTemperature()
{
    double temperature = _probe->getTemperature();
    qDebug("Probe: temperature is %.2f degC", temperature);
    return temperature;
}

//------------------------------------------------------------------------------

CameraCtrl::CameraCtrl(QObject *parent, CameraDriver *camera)
    : QObject(parent)
    , _camera(camera)
    , _exposure(-1)
    , _isAvailable(true)
{
    _camera->setParent(this);
    connect(_camera, CameraDriver::snapshotAvailable, this, processSnapshot);
}

//------------------------------------------------------------------------------

void CameraCtrl::setExposure(int exposure)
{
    Q_ASSERT(_isAvailable);

    if (_exposure != exposure)
    {
        qDebug("Camera: exposure time set to %d ms", exposure);
        _exposure = exposure;
        _camera->setExposure(exposure);
    }
}

//------------------------------------------------------------------------------

void CameraCtrl::takeSnapshot()
{
    Q_ASSERT(_isAvailable);
    Q_ASSERT(0 <= _exposure);

    qDebug("Camera: taking snapshot");
    _isAvailable = false;
    _camera->takeSnapshot();
}

//------------------------------------------------------------------------------

void CameraCtrl::stop()
{
    if (!_isAvailable)
    {
        qDebug("Camera: stop");
        _camera->stop();
        _isAvailable = true;
    }
}

//------------------------------------------------------------------------------

void CameraCtrl::getSnapshot(Snapshot &buffer)
{
    Q_ASSERT(_isAvailable);

    qDebug("Camera: getting snapshot");
    _camera->getSnapshot(buffer);
}

//------------------------------------------------------------------------------

void CameraCtrl::processSnapshot()
{
    qDebug("Camera: snapshot ready");
    _isAvailable = true;
    emit snapshotAvailable();
}

//------------------------------------------------------------------------------

}

#include "controllers.h"
#include "drivers.h"

#include <QDir> // Q_INIT_RESOURCE

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

FrequencyCtrl::FrequencyCtrl(QObject *parent, FrequencyDriver *generator)
    : QObject(parent)
    , _generator(generator)
    , _frequency(-1)
{
    _generator->setParent(this);
}

//------------------------------------------------------------------------------

bool FrequencyCtrl::setFrequency(double frequency)
{
    bool isUpdated = _frequency != frequency;

    if (isUpdated)
    {
        qDebug("Setting acoustic frequency to %.3f MHz", frequency);
        _frequency = frequency;
        _generator->setFrequency(_frequency);
    }

    return isUpdated;
}

//------------------------------------------------------------------------------

PowerCtrl::PowerCtrl(QObject *parent, PowerDriver *driver)
    : QObject(parent)
    , _driver(driver)
    , _power(-1.0)
{
    _driver->setParent(this);
}

//------------------------------------------------------------------------------

bool PowerCtrl::setPower(double power)
{
    bool isUpdated = _power != power;

    if (isUpdated)
    {
        qDebug("Setting acoustic power to %.1f mW", power);
        _power = power;
        _driver->setPower(_power);
    }

    return isUpdated;
}

//------------------------------------------------------------------------------

ProbeCtrl::ProbeCtrl(QObject *parent, ProbeDriver *probe)
    : QObject(parent)
    , _probe(probe)
{
  _probe->setParent(parent);
}

//------------------------------------------------------------------------------

double ProbeCtrl::getTemperature()
{
    double temperature = _probe->getTemperature();
    qDebug("Probe: temperature is %.2f degC", temperature);
    return temperature;
}

//------------------------------------------------------------------------------

CameraCtrl::CameraCtrl(QObject *parent, CameraDriver *cameraDevice)
    : QObject(parent)
    , _cameraDevice(cameraDevice)
    , _exposure(-1)
    , _isAvailable(true)
{
    _cameraDevice->setParent(this);
    connect(_cameraDevice, CameraDriver::snapshotAvailable, this, processSnapshot);
}

//------------------------------------------------------------------------------

void CameraCtrl::setExposure(int exposure)
{
    Q_ASSERT(_isAvailable);

    if (_exposure != exposure)
    {
        qDebug("Camera: exposure time set to %d ms", exposure);
        _exposure = exposure;
        _cameraDevice->setExposure(exposure);
    }
}

//------------------------------------------------------------------------------

void CameraCtrl::takeSnapshot()
{
    Q_ASSERT(_isAvailable);
    Q_ASSERT(0 <= _exposure);

    qDebug("Camera: taking snapshot");
    _isAvailable = false;
    _cameraDevice->takeSnapshot();
}

//------------------------------------------------------------------------------

void CameraCtrl::stop()
{
    if (!_isAvailable)
    {
        qDebug("Camera: stop");
        _cameraDevice->stop();
        _isAvailable = true;
    }
}

//------------------------------------------------------------------------------

void CameraCtrl::getSnapshot(Snapshot &buffer)
{
    Q_ASSERT(_isAvailable);

    qDebug("Camera: getting snapshot");
    _cameraDevice->getSnapshot(buffer);
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

#include "controllers.h"
#include "drivers.h"
#include <QDir> // Q_INIT_RESOURCE

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------
// AcousticCtrl
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
// ThermometerCtrl
//------------------------------------------------------------------------------

ThermometerCtrl::ThermometerCtrl(QObject *parent, ThermometerDriver *thermometer)
    : QObject(parent)
    , _thermometer(thermometer)
{
    _thermometer->setParent(this);
}

//------------------------------------------------------------------------------

double ThermometerCtrl::getTemperature()
{
    double temperature = _thermometer->getTemperature();
    qDebug("Probe: temperature is %.2f degC", temperature);
    return temperature;
}

//------------------------------------------------------------------------------
// CameraCtrl
//------------------------------------------------------------------------------

CameraCtrl::CameraCtrl(QObject *parent, CameraDriver *camera)
    : QObject(parent)
    , _camera(camera)
    , _exposure(-1)
    , _isBusy(false)
{
    _camera->setParent(this);
    connect(_camera, CameraDriver::snapshotAvailable, this, onSnapshotAvailable);
}

//------------------------------------------------------------------------------

void CameraCtrl::setExposure(int exposure)
{
    Q_ASSERT(!_isBusy);

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
    Q_ASSERT(!_isBusy);
    Q_ASSERT(0 <= _exposure);

    qDebug("Camera: taking snapshot");
    _isBusy = true;
    _camera->takeSnapshot();
}

//------------------------------------------------------------------------------

void CameraCtrl::stop()
{
    if (_isBusy)
    {
        qDebug("Camera: stop");
        _camera->stop();
        _isBusy = false;
    }
}

//------------------------------------------------------------------------------

void CameraCtrl::onSnapshotAvailable(const Snapshot &buffer)
{
    _isBusy = false;
    emit snapshotAvailable(buffer);
}

//------------------------------------------------------------------------------

}

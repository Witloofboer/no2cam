#include "Controllers.h"
#include "BaseDrivers.h"

#include <QDir> // Q_INIT_RESOURCE
#include <QTimer>
#include <QThread>

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------
// AcousticCtrl
//------------------------------------------------------------------------------

AcousticCtrl::AcousticCtrl(QObject *parent, BaseAcousticDriver *driver)
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

ThermometerCtrl::ThermometerCtrl(BaseThermometerDriver *thermometer)
    : QObject()
    , _thermometer(thermometer)
    , _thread(new QThread(this))
    , _thermometerT(new QTimer(this))
{
    _thermometer->setParent(this);
    connect(_thermometerT, QTimer::timeout, this, onTemperatureTimeout);
    this->moveToThread(_thread);
    _thread->start();
}

//------------------------------------------------------------------------------

void ThermometerCtrl::onTemperaturePeriodUpdated(int period)
{
    if (period != _thermometerT->interval())
    {
        qDebug("Setting thermometer period to %dms.", period);
        _thermometerT->stop();
        onTemperatureTimeout();
        _thermometerT->start(period);
    }
}

//------------------------------------------------------------------------------

void ThermometerCtrl::onShutdown()
{
    qDebug("ThermometerCtrl shutting down");
    _thermometerT->stop();
    _thread->quit();
}

//------------------------------------------------------------------------------

void ThermometerCtrl::onTemperatureTimeout()
{
    emit updateTemperature(_thermometer->getTemperature());
}

//------------------------------------------------------------------------------
// CameraCtrl
//------------------------------------------------------------------------------

CameraCtrl::CameraCtrl(QObject *parent, BaseCameraDriver *camera)
    : QObject(parent)
    , _camera(camera)
    , _exposure(-1)
    , _isBusy(false)
{
    _camera->setParent(this);
    connect(_camera, BaseCameraDriver::snapshotAvailable, this, onSnapshotAvailable);
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

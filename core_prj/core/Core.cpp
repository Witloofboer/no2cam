#include "Core.h"

#include <QCoreApplication>
#include <QTimer>

#include "AbstractCamera.h"
#include "Crystal.h"

//------------------------------------------------------------------------------

namespace core {

//------------------------------------------------------------------------------

Core::Core(const Crystal *crystal, AbstractCamera *camera)
    : QObject()
    , _cooldownTmr(new QTimer(this))
    , _crystal(crystal)
    , _camera(camera)
    , _mode(_kReady)
    , _bursting(false)
{
    camera->setParent(this);
    _cooldownTmr->setSingleShot(true);

    connect(_cooldownTmr, QTimer::timeout, this, continueAfterCooldown);
    connect(_camera, AbstractCamera::snapshotAvailable, this, processSnapshot);
}

//------------------------------------------------------------------------------

void Core::startSnapshot(double wavelength,
                         double frequency,
                         double power,
                         double exposure,
                         double cooldown,
                         bool burst,
                         const QString& session)
{
    QByteArray s = session.toLatin1();

    qInfo("Snapshot: wl=%.1f nm, fq=%.1f MHz, pwr=%.1f mW, expo=%.1f ms, "
          "cooldown=%.1f ms, %s, session='%s'",
          wavelength, frequency, power, exposure, cooldown,
          burst ? "burst" : "singleshot", s.data());

    _mode = _kSnapshotting;
    _bursting = burst;
    _cooldownTmr->setInterval(cooldown);
    _camera->setExposure(exposure);

    // TODO set frequency and power
    continueAfterCooldown();
}

//------------------------------------------------------------------------------

void Core::startObservation(double wavelength1,
                            double wavelength2,
                            double exposure,
                            int snapshotPerObs,
                            double cooldown,
                            bool burst,
                            const QString &session)
{
    QByteArray s = session.toLatin1();

    qInfo("Observation: wl1=%.1f nm, wl2=%.1f nm, expo=%.1f ms, s/obs=%d, "
          "cooldown=%.1f ms, %s, session='%s'",
          wavelength1, wavelength2, exposure, snapshotPerObs, cooldown,
          burst ? "burst" : "singleshot", s.data());

    _mode = _kObserving;
    _bursting = burst;
    _cooldownTmr->setInterval(cooldown);
    _camera->setExposure(exposure);
    _wavelengths[0] = wavelength1;
    _wavelengths[1] = wavelength2;
    _wavelengthIx = 0;
    _snapshotCount = 0;
    _snapshotPerObs = snapshotPerObs;

    // TODO set frequency and power
    continueAfterCooldown();
}


//------------------------------------------------------------------------------

void Core::startSweep(double wavelength1,
                      double wavelength2,
                      double wavelengthStep,
                      double exposure,
                      double cooldown,
                      bool burst,
                      const QString &session)
{
    QByteArray s = session.toLatin1();

    qInfo("Sweep: wl1=%.1f nm, wl2=%.1f nm, step=%.1f nm, expo=%.1f ms, "
          "cooldown=%.1f ms, %s, session='%s'",
          wavelength1, wavelength2, wavelengthStep, exposure, cooldown,
          burst ? "burst" : "singleshot", s.data());

    _mode = _kSweeping;
    _bursting = burst;
    _cooldownTmr->setInterval(cooldown);
    _camera->setExposure(exposure);
    _wavelengths[0] = wavelength1;
    _wavelengths[1] = wavelength2;
    _wavelength = wavelength1;
    _wavelengthStep = wavelengthStep;

    // TODO set frequency and power
    continueAfterCooldown();
}

//------------------------------------------------------------------------------

void Core::stop() // Todo
{
    qInfo("Stopping all devices");
    _camera->stop();
    _cooldownTmr->stop();
    _mode = _kReady;

    emit ready(true);
}

//------------------------------------------------------------------------------

void Core::moveToMainThread()
{
    qInfo("Moving core layer back to main thread");
    moveToThread(QCoreApplication::instance()->thread());
}

//------------------------------------------------------------------------------

void Core::processSnapshot()
{
    qInfo("Processing snapshot");

    switch(_mode)
    {
    case _kReady:
        break;

    case _kSnapshotting:
        // TODO process
        if (_bursting)
        {
            cooldown();
        } else {
            stop();
        }
        break;

    case _kObserving:
        // TODO process

        ++_snapshotCount;
        _wavelengthIx = 1 - _wavelengthIx;

        if (_bursting || _snapshotCount < 2*_snapshotPerObs)
        {
            if (_snapshotCount == 2*_snapshotPerObs)
            {
                _snapshotPerObs = 0;
            }

            cooldown();
        } else {
            stop();
        }
        break;

    case _kSweeping:
        if (_bursting || _wavelength < _wavelengths[1])
        {
            if (_wavelength < _wavelengths[1])
            {
                _wavelength += _wavelengthStep;
            } else {
                _wavelength = _wavelengths[0];
            }

            cooldown();
        } else {
            stop();
        }
        break;
    }
}

//------------------------------------------------------------------------------

void Core::cooldown() // Todo
{
    qInfo("Cooling down");
    // TODO reduce power
    _cooldownTmr->start();
}

//------------------------------------------------------------------------------

void Core::continueAfterCooldown()
{
    // TODO set power
    switch(_mode)
    {
    case _kReady:
        break;

    case _kSnapshotting:
        qInfo("Snapshotting");
        _camera->takeSnapshot();
        break;

    case _kObserving:
        qInfo("Snapshotting at %f nm", _wavelengths[_wavelengthIx]);
        // TODO set frequency
        // TODO set power
        _camera->takeSnapshot();
        break;

    case _kSweeping:
        qInfo("Sweeping at %f nm", _wavelength);
        // TODO set frequency
        // TODO set power
        _camera->takeSnapshot();
        break;
    }
}

//------------------------------------------------------------------------------

}


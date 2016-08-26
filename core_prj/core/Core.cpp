#include "Core.h"

#include <QCoreApplication>
#include <QTimer>

#include "AbstractCamera.h"
#include "Crystal.h"

//------------------------------------------------------------------------------

namespace core {

//------------------------------------------------------------------------------

Core::Core(const Crystal *crystal,
           AbstractCamera *camera,
           AbstractGenerator *generator,
           AbstractDriver *driver)
    : QObject()
    , _cooldownT(new QTimer(this))
    , _relaxT(new QTimer(this))
    , _crystal(crystal)
    , _camera(camera)
    , _generator(generator)
    , _driver(driver)
    , _mode(_kReady)
    , _bursting(false)
{
    camera->setParent(this);
    _cooldownT->setSingleShot(true);
    _relaxT->setSingleShot(true);

    connect(_cooldownT, QTimer::timeout, this, setAcousticWave);
    connect(_relaxT, QTimer::timeout, _camera, takeSnapshot);
    connect(_camera, AbstractCamera::snapshotAvailable, this, postSnapshotProcess);
}

//------------------------------------------------------------------------------

void Core::startSnapshot(double wavelength,
                         double frequency,
                         double power,
                         double exposure,
                         double cooldown,
                         bool burst,
                         double relaxTime,
                         const QString& session)
{
    QByteArray s = session.toLatin1();

    qInfo("Snapshot: wl=%.1f nm, fq=%.1f MHz, pwr=%.1f mW, expo=%.1f ms, "
          "cooldown=%.1f ms, %s, session='%s'",
          wavelength, frequency, power, exposure, cooldown,
          burst ? "burst" : "singleshot", s.data());

    _mode = _kSnapshotting;
    _bursting = burst;
    _cooldownT->setInterval(cooldown);
    _relaxT->setInterval(relaxTime);
    _camera->setExposure(exposure);
    _generator->setFrequency(frequency);
    _p.snap.power = power;

    setAcousticWave();
}

//------------------------------------------------------------------------------

void Core::startObservation(double wavelength1,
                            double wavelength2,
                            double exposure,
                            int snapshotPerObs,
                            double cooldown,
                            bool burst,
                            double relaxTime,
                            const QString &session)
{
    QByteArray s = session.toLatin1();

    qInfo("Observation: wl1=%.1f nm, wl2=%.1f nm, expo=%.1f ms, s/obs=%d, "
          "cooldown=%.1f ms, %s, session='%s'",
          wavelength1, wavelength2, exposure, snapshotPerObs, cooldown,
          burst ? "burst" : "singleshot", s.data());

    _mode = _kObserving;
    _bursting = burst;
    _cooldownT->setInterval(cooldown);
    _relaxT->setInterval(relaxTime);
    _camera->setExposure(exposure);
    _p.obs.wavelengths[0] = wavelength1;
    _p.obs.wavelengths[1] = wavelength2;
    _p.obs.wavelengthIx = 0;
    _p.obs.snapshotCount = 0;
    _p.obs.snapshotPerObs = snapshotPerObs;

    setAcousticWave();
}


//------------------------------------------------------------------------------

void Core::startSweep(double wavelength1,
                      double wavelength2,
                      double wavelengthStep,
                      double exposure,
                      double cooldown,
                      bool burst,
                      double relaxTime,
                      const QString &session)
{
    QByteArray s = session.toLatin1();

    qInfo("Sweep: wl1=%.1f nm, wl2=%.1f nm, step=%.1f nm, expo=%.1f ms, "
          "cooldown=%.1f ms, %s, session='%s'",
          wavelength1, wavelength2, wavelengthStep, exposure, cooldown,
          burst ? "burst" : "singleshot", s.data());

    _mode = _kSweeping;
    _bursting = burst;
    _cooldownT->setInterval(cooldown);
    _relaxT->setInterval(relaxTime);
    _camera->setExposure(exposure);
    _p.sweep.minWavelength = wavelength1;
    _p.sweep.maxWavelength = wavelength2;
    _p.sweep.wavelength = wavelength1;
    _p.sweep.wavelengthStep = wavelengthStep;

    // TODO set frequency and power
    setAcousticWave();
}

//------------------------------------------------------------------------------

void Core::stop() // Todo
{
    qInfo("Stopping all devices");
    _camera->stop();
    _cooldownT->stop();
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

void Core::setAcousticWave()
{
    switch(_mode)
    {
    case _kReady:
        break;

    case _kSnapshotting:
        _driver->setPower(_p.snap.power);
        break;

    case _kObserving:
        // TODO set frequency
        // TODO set power
        break;

    case _kSweeping:
        // TODO set frequency
        // TODO set power
        break;
    }

    _relaxT->start();
}

//------------------------------------------------------------------------------

void Core::postSnapshotProcess()
{
    bool continueAquisition = _bursting;

    switch(_mode)
    {
    case _kReady:
        break;

    case _kSnapshotting:
        break;

    case _kObserving:
        ++_snapshotCount;
        _wavelengthIx = 1 - _wavelengthIx;

        continueAquisition |= _p.obs.snapshotCount < 2*_p.obs.snapshotPerObs;

        if (_p.obs.snapshotCount == 2*_p.obs.snapshotPerObs)
        {
            _p.obs.snapshotPerObs = 0;
        }
        break;

    case _kSweeping:
        continueAquisition |= _p.sweep.wavelength < _p.sweep.maxWavelength;

        if (_p.sweep.wavelength < _p.sweep.maxWavelength)
        {
            _p.sweep.wavelength += _p.sweep.wavelengthStep;
        } else {
            _p.sweep.wavelength = _p.sweep.minWavelength;
        }
        break;
    }

    if (continueAquisition)
    {
        _driver->cooldownPower();
        _cooldownT->start();
    } else {
        stop();
    }
}

//------------------------------------------------------------------------------

}


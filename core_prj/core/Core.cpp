#include "Core.h"

#include <QCoreApplication>
#include <QTimer>

#include "AbstractCamera.h"
#include "AbstractCrysTempProbe.h"
#include "AbstractDriver.h"
#include "AbstractGenerator.h"
#include "Crystal.h"

//------------------------------------------------------------------------------

namespace core {

//------------------------------------------------------------------------------

Core::Core(const Crystal *crystal,
           AbstractCrysTempProbe *crysTempProb,
           AbstractCamera *camera,
           AbstractGenerator *generator,
           AbstractDriver *driver)
    : QObject()
    , _cooldownT(new QTimer(this))
    , _stabilisationT(new QTimer(this))
    , _crystal(crystal)
    , _crysTempProb(crysTempProb)
    , _camera(camera)
    , _generator(generator)
    , _driver(driver)
    , _mode(READY)
    , _bursting(false)
    , _session()
{
    camera->setParent(this);
    _cooldownT->setSingleShot(true);
    _stabilisationT->setSingleShot(true);

    connect(_cooldownT, QTimer::timeout, this, setAcousticWave);
    connect(_stabilisationT, QTimer::timeout, _camera, AbstractCamera::takeSnapshot);
    connect(_camera, AbstractCamera::snapshotAvailable, this, postSnapshotProcess);
}

//------------------------------------------------------------------------------

void Core::spectralSnapshot(double wavelength,
                            double exposure,
                            double cooldown,
                            double stabilisationTime,
                            bool burst,
                            const QString& session)
{
    qInfo("Spectral snap: wl=%.1f nm", wavelength);
    setCommonParams(SpSNAP, exposure, cooldown, stabilisationTime, burst, session);

    _p.specSnap.wavelength = wavelength;

    setAcousticWave();
}

//------------------------------------------------------------------------------

void Core::acousticSnapshot(double frequency,
                            double power,
                            double exposure,
                            double cooldown,
                            double stabilisationTime,
                            bool burst,
                            const QString& session)
{
    qInfo("Acoustic snap: freq=%.1f MHz, power=%.1f mW", frequency, power);

    setCommonParams(AcSNAP, exposure, cooldown, stabilisationTime, burst, session);
    _p.acouSnap.frequency = frequency;
    _p.acouSnap.power = power;

    setAcousticWave();
}

//------------------------------------------------------------------------------

void Core::observation(double wavelength1,
                       double wavelength2,
                       double exposure,
                       int snapshotPerObs,
                       double cooldown,
                       double stabilisationTime,
                       bool burst,
                       const QString &session)
{
    qInfo("Observation: wl1=%.1f nm, wl2=%.1f nm",
          wavelength1, wavelength2);

    setCommonParams(OBS, exposure, cooldown, stabilisationTime, burst, session);
    _p.obs.wavelengths[0] = wavelength1;
    _p.obs.wavelengths[1] = wavelength2;
    _p.obs.wavelengthIx = 0;
    _p.obs.snapshotCount = 0;
    _p.obs.snapshotPerObs = snapshotPerObs;

    setAcousticWave();
}


//------------------------------------------------------------------------------

void Core::sweep(double wavelength1,
                 double wavelength2,
                 double wavelengthStep,
                 double exposure,
                 double cooldown,
                 double stabilisationTime,
                 bool burst,
                 const QString &session)
{
    qInfo("Sweep: wl1=%.1f nm, wl2=%.1f nm, step=%.1f nm",
          wavelength1, wavelength2, wavelengthStep);

    setCommonParams(SWP, exposure, cooldown, stabilisationTime, burst, session);

    _p.swp.minWavelength = wavelength1;
    _p.swp.maxWavelength = wavelength2;
    _p.swp.wavelength = wavelength1;
    _p.swp.wavelengthStep = wavelengthStep;

    setAcousticWave();
}

//------------------------------------------------------------------------------

void Core::stop()
{
    _driver->stop();
    _generator->stop();
    _camera->stop();
    _cooldownT->stop();
    _stabilisationT->stop();
    _mode = READY;

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
    qInfo("Setting the acousting power");
    switch(_mode)
    {
    case READY:
        break;

    case SpSNAP:
        setOptimalAcousticWave(_p.specSnap.wavelength);
        break;

    case AcSNAP:
        _generator->setFrequency(_p.acouSnap.frequency);
        _driver->setPower(_p.acouSnap.power);
        break;

    case OBS:
        setOptimalAcousticWave(_p.obs.wavelengths[_p.obs.wavelengthIx]);
        break;

    case SWP:
        setOptimalAcousticWave(_p.swp.wavelength);
        break;
    }

    switch(_mode)
    {
    case READY:
        break;

    case SpSNAP:
    case AcSNAP:
    case OBS:
    case SWP:
        qInfo("Waiting for acoustic wave stabilisation");
        _stabilisationT->start();
        break;
    }
}

//------------------------------------------------------------------------------

void Core::setOptimalAcousticWave(double wavelength)
{
    const double temp = _crysTempProb->getTemperature();
    double freq, power;

    _crystal->computeFreqPow(wavelength, temp, freq, power);

    _generator->setFrequency(freq);
    _driver->setPower(power);
}

//------------------------------------------------------------------------------

void Core::postSnapshotProcess()
{
    bool continueAquisition = _bursting;

    switch(_mode)
    {
    case READY:
        break;

    case SpSNAP:
    case AcSNAP:
        break;

    case OBS:
        ++_p.obs.snapshotCount;
        _p.obs.wavelengthIx = 1 - _p.obs.wavelengthIx;

        continueAquisition |= _p.obs.snapshotCount < 2*_p.obs.snapshotPerObs;

        if (_p.obs.snapshotCount == 2*_p.obs.snapshotPerObs)
        {
            _p.obs.snapshotPerObs = 0;
        }
        break;

    case SWP:
        continueAquisition |= _p.swp.wavelength < _p.swp.maxWavelength;

        if (_p.swp.wavelength < _p.swp.maxWavelength)
        {
            _p.swp.wavelength += _p.swp.wavelengthStep;
        } else {
            _p.swp.wavelength = _p.swp.minWavelength;
        }
        break;
    }

    if (continueAquisition)
    {
        _driver->cooldown();
        _cooldownT->start();
    } else {
        stop();
    }
}

//------------------------------------------------------------------------------

void Core::setCommonParams(Mode mode,
                           double exposure,
                           double cooldown,
                           double stabilisationTime,
                           bool burst,
                           const QString &session)
{
    QByteArray s = session.toLatin1();

    qInfo("Exposure=%.1f ms, cooldown=%.1f ms, relax time=%.1f ms, "
          "%s, session='%s'",
          exposure, cooldown, stabilisationTime,
          burst ? "burst" : "singleshot", s.data());

    _mode = mode;
    _camera->setExposure(exposure);
    _cooldownT->setInterval(cooldown);
    _stabilisationT->setInterval(stabilisationTime);
    _bursting = burst;
    _session = session;
}

//------------------------------------------------------------------------------

}

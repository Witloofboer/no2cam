#include "Core.h"

#include <QCoreApplication>
#include <QFile>
#include <QTimer>

#include "BaseCamera.h"
#include "AbstractCrysTempProbe.h"
#include "AbstractDriver.h"
#include "AbstractGenerator.h"
#include "Crystal.h"
#include "ImageBuffer.h"

//------------------------------------------------------------------------------

namespace core {

//------------------------------------------------------------------------------

Core::Core(const Crystal *crystal,
           AbstractCrysTempProbe *crysTempProb,
           BaseCamera *camera,
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
    , _snapshot{0}
    , _snapTime()
    , _session()
{
    camera->setParent(this); // TODO setParent for the other devices
    _cooldownT->setSingleShot(true);
    _stabilisationT->setSingleShot(true);

    connect(_cooldownT, QTimer::timeout, this, setAcousticWave);
    connect(_stabilisationT, QTimer::timeout, this, takeSnapshot);
    connect(_camera, BaseCamera::snapshotAvailable, this, postSnapshotProcess);
}

//------------------------------------------------------------------------------

void Core::spectralSnapshot(double wavelength,
                            int exposure,
                            int cooldownTime,
                            int cooldownPwr,
                            int stabilisationTime,
                            bool burst,
                            const QString& session)
{
    qInfo("Spectral snap: wl=%.1f nm", wavelength);
    setCommonParams(SpectralSnap, exposure, cooldownTime, cooldownPwr,
                    stabilisationTime, burst, session);

    auto &p = _p.specSnap;
    p.in_wavelength = wavelength;

    setAcousticWave();
}

//------------------------------------------------------------------------------

void Core::acousticSnapshot(double frequency,
                            double power,
                            int exposure,
                            int cooldownTime,
                            int cooldownPwr,
                            int stabilisationTime,
                            bool burst,
                            const QString& session)
{
    qInfo("Acoustic: freq=%.1f MHz, power=%.1f mW", frequency, power);

    setCommonParams(AcousticSnap, exposure, cooldownTime, cooldownPwr,
                    stabilisationTime, burst, session);

    auto &p = _p.acouSnap;
    p.in_frequency = frequency;
    p.in_power = power;

    setAcousticWave();
}

//------------------------------------------------------------------------------

void Core::observation(double wavelength1,
                       double wavelength2,
                       int exposure,
                       int snapshotPerObs,
                       int cooldownTime,
                       int cooldownPwr,
                       int stabilisationTime,
                       bool burst,
                       const QString &session)
{
    qInfo("Observation: wl1=%.1f nm, wl2=%.1f nm",
          wavelength1, wavelength2);

    setCommonParams(Obs, exposure, cooldownTime, cooldownPwr,
                    stabilisationTime, burst, session);

    auto &p = _p.obs;
    p.in_wavelengths[0] = wavelength1;
    p.in_wavelengths[1] = wavelength2;
    p.idx = 0;
    p.snapshotCount = 0;
    p.in_snapshotPerObs = snapshotPerObs;

    setAcousticWave();
}


//------------------------------------------------------------------------------

void Core::sweep(double wavelength1,
                 double wavelength2,
                 double wavelengthStep,
                 int exposure,
                 int cooldownTime,
                 int cooldownPwr,
                 int stabilisationTime,
                 bool burst,
                 const QString &session)
{
    qInfo("Sweep: wl1=%.1f nm, wl2=%.1f nm, step=%.1f nm",
          wavelength1, wavelength2, wavelengthStep);

    setCommonParams(Sweep, exposure, cooldownTime, cooldownPwr,
                    stabilisationTime, burst, session);

    auto &p = _p.swp;
    p.in_minWavelength = wavelength1;
    p.in_maxWavelength = wavelength2;
    p.in_wavelengthStep = wavelengthStep;
    p.wavelength = wavelength1;

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
    if (_mode == READY)
        return;

    switch(_mode)
    {
    case READY:
        Q_UNREACHABLE();
        break;

    case SpectralSnap:
    {
        auto &p = _p.specSnap;

        p.temperature = _crysTempProb->getTemperature();
        _crystal->computeFreqPow(p.in_wavelength,
                                 p.temperature,
                                 p.frequency,
                                 p.power);
        _generator->setFrequency(p.frequency);
        _driver->setPower(p.power);

        break;
    }

    case AcousticSnap:
    {
        auto &p = _p.acouSnap;

        p.temperature = _crysTempProb->getTemperature();
        p.wavelength = _crystal->wavelength(p.in_frequency,
                                            p.temperature);
        _generator->setFrequency(p.in_frequency);
        _driver->setPower(p.in_power);

        break;
    }

    case Obs:
    {
        auto &p = _p.obs;

        if (p.snapshotCount == 0)
        {
            // First snapshot... initialisation of the observation
            for(int i=0; i<BaseCamera::size; ++i)
                for(int j=0; j<BaseCamera::size; ++j)
                {
                    p.snapshots[0][i][j] = 0;
                    p.snapshots[1][i][j] = 0;
                }

            p.temperature = _crysTempProb->getTemperature();

            for(int i=0; i<2; ++i)
            {
                _crystal->computeFreqPow(p.in_wavelengths[i],
                                         p.temperature,
                                         p.frequency[i],
                                         p.power[i]);
            }
        }

        _generator->setFrequency(p.frequency[p.idx]);
        _driver->setPower(p.power[p.idx]);

        break;
    }

    case Sweep:
    {
        auto &p = _p.swp;

        p.temperature = _crysTempProb->getTemperature();
        _crystal->computeFreqPow(p.wavelength,
                                 p.temperature,
                                 p.frequency,
                                 p.power);
        _generator->setFrequency(p.frequency);
        _driver->setPower(p.power);

        break;
    }
    }

    qInfo("Waiting for acoustic wave stabilisation");
    _stabilisationT->start();

}

//------------------------------------------------------------------------------

void Core::takeSnapshot()
{
    if (_mode == READY)
        return;

    if (_mode != Obs || _p.obs.snapshotCount == 0)
        _snapTime = QDateTime::currentDateTime();

    _camera->takeSnapshot();
}

//------------------------------------------------------------------------------

void Core::postSnapshotProcess()
{
    _camera->copySnapshot(_snapshot);

    bool continueAquisition = _bursting;

    switch(_mode)
    {
    case READY:
        break;

    case SpectralSnap:
    {
        auto &p = _p.specSnap;

        gImageBuffer.set(_snapshot);
        emit snapshotAvailable();

        saveSnapshot(_snapTime,
                     SpectralSnap,
                     p.in_wavelength,
                     p.frequency,
                     p.power,
                     p.temperature,
                     _snapshot);
        break;
    }
    case AcousticSnap:
    {
        auto &p = _p.acouSnap;

        gImageBuffer.set(_snapshot);
        emit snapshotAvailable();

        saveSnapshot(_snapTime,
                     SpectralSnap,
                     p.wavelength,
                     p.in_frequency,
                     p.in_power,
                     p.temperature,
                     _snapshot);
        break;
    }

    case Obs:
    {
        auto &p = _p.obs;

        // Add the current snapshot to the consolidated one.
        for(int i=0; i<BaseCamera::size; ++i)
            for(int j=0; j<BaseCamera::size; ++j)
                p.snapshots[p.idx][i][j] += _snapshot[i][j];

        ++p.snapshotCount;
        p.idx = 1 - p.idx;

        if (p.snapshotCount == 2*p.in_snapshotPerObs)
        {
            p.snapshotCount = 0;

            for(int i=0; i<2; ++i)
                saveSnapshot(_snapTime,
                             Obs,
                             p.in_wavelengths[i],
                             p.frequency[i],
                             p.power[i],
                             p.temperature,
                             p.snapshots[i]);

            for(int i=0; i<BaseCamera::size; ++i)
                for(int j=0; j<BaseCamera::size; ++j)
                    _snapshot[i][j]=(p.snapshots[0][i][j]+p.snapshots[1][i][j])/2;

            gImageBuffer.set(_snapshot);
            emit snapshotAvailable();

        }

        continueAquisition |= p.snapshotCount < 2*p.in_snapshotPerObs;

        break;
    }

    case Sweep:
    {
        auto &p = _p.swp;

        saveSnapshot(_snapTime,
                     Sweep,
                     p.wavelength,
                     p.frequency,
                     p.power,
                     p.temperature,
                     _snapshot);

        gImageBuffer.set(_snapshot);
        emit snapshotAvailable();

        if (p.wavelength < p.in_maxWavelength)
        {
            p.wavelength += p.in_wavelengthStep;
        }
        else
        {
            p.wavelength = p.in_minWavelength;
        }

        continueAquisition |= p.wavelength < p.in_maxWavelength;

        break;
    }
    }

    if (continueAquisition)
    {
        _driver->setPower(_cooldownPwr);
        _cooldownT->start();
    } else {
        stop();
    }
}

//------------------------------------------------------------------------------


void Core::saveSnapshot(const QDateTime& dateTime,
                        Mode mode,
                        double wavelength,
                        double frequency,
                        double power,
                        double temperature,
                        BaseCamera::Snapshot& snapshot)
{
    if (_session.isEmpty()) return;

    const QChar zero('0');
    auto _filename = QString("%1-%2-%3-%4nm-%5Ghz-%6mW-%7ms-%8degC.dat")
                     . arg(_session)
                     . arg(dateTime.toString("yyMMdd-HHmmss.zzz"))
                     . arg(_modeToCode[mode])
                     . arg(wavelength, 5, 'f', 1, zero)
                     . arg(frequency, 7, 'f', 3, zero)
                     . arg(power, 3, 'f', 0, zero)
                     . arg(_exposure, 4, 10, zero)
                     . arg(temperature, 4, 'f', 1, zero);

    QFile file(_filename);
    qInfo("Writing snapshot on disk");
    file.open(QIODevice::WriteOnly);
    const qint64 size = BaseCamera::size;

    file.write(reinterpret_cast<const char*>(snapshot), size*size*2);
    file.close();
}

//------------------------------------------------------------------------------

void Core::setCommonParams(Mode mode,
                           int exposure,
                           int cooldownTime,
                           int cooldownPwr,
                           int stabilisationTime,
                           bool burst,
                           const QString &session)
{
    QByteArray s = session.toLatin1();

    qInfo("Exposure=%d ms, cooldownTime=%d ms, %d mW, "
          "stabilisation time=%d ms, %s, session='%s'",
          exposure, cooldownTime, cooldownPwr, stabilisationTime,
          burst ? "burst" : "singleshot", s.data());

    _mode = mode;
    _exposure=exposure;
    _camera->setExposure(exposure);
    _cooldownT->setInterval(cooldownTime);
    _cooldownPwr = cooldownPwr;
    _stabilisationT->setInterval(stabilisationTime);
    _bursting = burst;
    _session = session;
}

//------------------------------------------------------------------------------

}

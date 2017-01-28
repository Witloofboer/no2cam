#include "Core.h"

#include <QCoreApplication>
#include <QFile>
#include <QTimer>

#include "BaseTemperatureProbe.h"
#include "BaseCamera.h"
#include "BaseDriver.h"
#include "BaseGenerator.h"
#include "Crystal.h"
#include "ImageBuffer.h"

//------------------------------------------------------------------------------

namespace core {

//------------------------------------------------------------------------------

Core::Core(const Crystal *crystal,
           BaseTemperatureProbe *crysTempProb,
           BaseCamera *camera,
           BaseGenerator *generator,
           BaseDriver *driver)
    : QObject()
    , _cooldownT(new QTimer(this))
    , _stabilisationT(new QTimer(this))
    , _temperatureT(new QTimer(this))
    , _crystal(crystal)
    , _temperatureProbe(crysTempProb)
    , _camera(camera)
    , _generator(generator)
    , _driver(driver)
    , _mode(READY)
    , _bursting(false)
    , _snapshotBuffer{0}
{
    camera->setParent(this); // TODO setParent for the other devices
    _cooldownT->setSingleShot(true);
    _stabilisationT->setSingleShot(true);

    connect(_cooldownT, QTimer::timeout, this, setAcousticWave);
    connect(_stabilisationT, QTimer::timeout, this, takeSnapshot);
    connect(_temperatureT, QTimer::timeout, this, updateTemperature);
    connect(_camera, BaseCamera::snapshotAvailable, this, postSnapshotProcess);
}

//------------------------------------------------------------------------------

void Core::spectralSnapshot(double wavelength,
                            int exposure,
                            int cooldownTime,
                            int cooldownPwr,
                            int stabilisationTime,
                            bool burst,
                            bool record,
                            QString dataFolder,
                            QString session)
{
    qDebug("Spectral snap: wl=%.1f nm", wavelength);
    setCommonParams(SpectralSnap, exposure, cooldownTime, cooldownPwr,
                    stabilisationTime, burst, record, dataFolder, session);

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
                            bool record,
                            QString dataFolder,
                            QString session)
{
    qDebug("Acoustic: freq=%.1f MHz, power=%.1f mW", frequency, power);

    setCommonParams(AcousticSnap, exposure, cooldownTime, cooldownPwr,
                    stabilisationTime, burst, record, dataFolder, session);

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
                       bool record,
                       QString dataFolder,
                       QString session)
{
    qDebug("Observation: wl1=%.1f nm, wl2=%.1f nm",
           wavelength1, wavelength2);

    setCommonParams(Obs, exposure, cooldownTime, cooldownPwr,
                    stabilisationTime, burst, record, dataFolder, session);

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
                 bool record,
                 QString dataFolder,
                 QString session)
{
    qDebug("Sweep: wl1=%.1f nm, wl2=%.1f nm, step=%.1f nm",
           wavelength1, wavelength2, wavelengthStep);

    setCommonParams(Sweep, exposure, cooldownTime, cooldownPwr,
                    stabilisationTime, burst, record, dataFolder, session);

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
    _generator->setFrequency(0.0);
    _driver->setPower(0.0);
    _camera->stop();
    _cooldownT->stop();
    _stabilisationT->stop();
    _mode = READY;

    emit ready(true);
}

//------------------------------------------------------------------------------

void Core::updateTemperaturePeriod(int temperaturePeriod)
{
    qDebug("Setting temperature probe period to %dms.", temperaturePeriod);
    _temperatureT->stop();
    updateTemperature();
    _temperatureT->start(temperaturePeriod);

}

//------------------------------------------------------------------------------

void Core::threadFinished()
{
    qDebug("Moving core layer back to main thread");
    _temperatureT->stop();
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

        _crystal->computeFreqPow(p.in_wavelength,
                                 _temperature,
                                 p.frequency,
                                 p.power);
        requestAcousticWave(p.frequency, p.power);
        break;
    }

    case AcousticSnap:
    {
        auto &p = _p.acouSnap;

        p.wavelength = _crystal->wavelength(p.in_frequency,
                                            _temperature);
        requestAcousticWave(p.in_frequency, p.in_power);
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

            for(int i=0; i<2; ++i)
            {
                _crystal->computeFreqPow(p.in_wavelengths[i],
                                         _temperature,
                                         p.frequency[i],
                                         p.power[i]);
            }
        }

        requestAcousticWave(p.frequency[p.idx], p.power[p.idx]);
        break;
    }

    case Sweep:
    {
        auto &p = _p.swp;

        _crystal->computeFreqPow(p.wavelength,
                                 _temperature,
                                 p.frequency,
                                 p.power);
        requestAcousticWave(p.frequency, p.power);
        break;
    }
    }
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


void Core::updateTemperature()
{
    _temperature = _temperatureProbe->getTemperature();
    emit temperatureUpdated(_temperature);
}

//------------------------------------------------------------------------------

void Core::postSnapshotProcess()
{
    if (_mode==READY) return;

    _camera->getSnapshot(_snapshotBuffer);

    switch(_mode)
    {
    case READY:
        Q_UNREACHABLE();
        break;

    case SpectralSnap:
    {
        auto &p = _p.specSnap;

        gImageBuffer.set(_snapshotBuffer);
        emit snapshotAvailable();

        saveSnapshot(_snapTime,
                     SpectralSnap,
                     p.in_wavelength,
                     p.frequency,
                     p.power,
                     1,
                     _exposure,
                     _temperature,
                     _snapshotBuffer);
        break;
    }

    case AcousticSnap:
    {
        auto &p = _p.acouSnap;

        gImageBuffer.set(_snapshotBuffer);
        emit snapshotAvailable();

        saveSnapshot(_snapTime,
                     AcousticSnap,
                     p.wavelength,
                     p.in_frequency,
                     p.in_power,
                     1,
                     _exposure,
                     _temperature,
                     _snapshotBuffer);
        break;
    }

    case Obs:
    {
        auto &p = _p.obs;

        // Add the current snapshot to the consolidated one.
        for(int i=0; i<BaseCamera::size; ++i)
            for(int j=0; j<BaseCamera::size; ++j)
                p.snapshots[p.idx][i][j] += _snapshotBuffer[i][j];

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
                             p.in_snapshotPerObs,
                             _exposure,
                             p.temperature,
                             p.snapshots[i]);

            for(int i=0; i<BaseCamera::size; ++i)
                for(int j=0; j<BaseCamera::size; ++j)
                    _snapshotBuffer[i][j]=(p.snapshots[0][i][j]+p.snapshots[1][i][j])/2;

            gImageBuffer.set(_snapshotBuffer);
            emit snapshotAvailable();
        }
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
                     1,
                     _exposure,
                     _temperature,
                     _snapshotBuffer);

        gImageBuffer.set(_snapshotBuffer);
        emit snapshotAvailable();

        if (p.wavelength < p.in_maxWavelength)
        {
            p.wavelength += p.in_wavelengthStep;
        } else {
            p.wavelength = p.in_minWavelength;
        }
        break;
    }
    }

    if (mustContinueAquisition())
    {
        if (mustCooldown())
        {
            this->cooldown();
        } else {
            setAcousticWave();
        }
    } else {
        stop();
    }
}

//------------------------------------------------------------------------------

bool Core::mustContinueAquisition() const
{
    switch(_mode)
    {
    case READY:
        return false;

    case SpectralSnap:
    case AcousticSnap:
        return _bursting;

    case Obs:
        return _bursting || _p.obs.snapshotCount != 0;

    case Sweep:
        return _bursting || _p.swp.wavelength < _p.swp.in_maxWavelength;
    }
    Q_UNREACHABLE();
}

//------------------------------------------------------------------------------

bool Core::mustCooldown() const
{
    switch(_mode)
    {
    case READY:
        Q_UNREACHABLE();

    case SpectralSnap:
    case AcousticSnap:
    case Sweep:
        return _cooldownT->interval() != 0;

    case Obs:
        return _cooldownT->interval() != 0 && _p.obs.snapshotCount == 0;
    }
    Q_UNREACHABLE();
}

//------------------------------------------------------------------------------

void Core::cooldown()
{
    qDebug("Cooling down: %dms", _cooldownT->interval());
    _driver->setPower(_cooldownPwr);
    _cooldownT->start();
}

//------------------------------------------------------------------------------

void Core::saveSnapshot(const QDateTime& dateTime,
                        Mode mode,
                        double wavelength,
                        double frequency,
                        double power,
                        int snapPerObs,
                        int exposure,
                        double temperature,
                        BaseCamera::Snapshot& snapshot)
{
    if (!_record) return;

    const QChar zero('0');

    QString expo = (mode==Obs)
                   ? QString("%1x%2").arg(snapPerObs).arg(exposure)
                   : QString("%1").arg(exposure);

    auto _filename = QString("%1-%2-%3-%4nm-%5Ghz-%6mW-%7ms-%8degC.dat")
                     . arg(dateTime.toString("yyMMdd-HHmmss.zzz"))
                     . arg(_session.isEmpty() ? "_" : _session)
                     . arg(_modeToCode[mode])
                     . arg(wavelength, 1, 'f', 1, zero)
                     . arg(frequency, 1, 'f', 3, zero)
                     . arg(power, 1, 'f', 0, zero)
                     . arg(expo)
                     . arg(temperature, 1, 'f', 1, zero);

    QFile file(_dataFolder+"/"+_filename);
    qInfo("Dumping snapshot to %s", _filename.toLatin1().constData());

    const bool ok = file.open(QIODevice::WriteOnly);

    if (! ok)
    {
        stop();
        emit errorOnFileCreation(_dataFolder, _filename);
        return;
    }

    auto buf = reinterpret_cast<const char*>(snapshot);
    const qint64 bufSize = BaseCamera::size*BaseCamera::size*2;

    const qint64 onDisk = file.write(buf, bufSize);

    file.close();

    if (bufSize != onDisk)
    {
        stop();
        emit errorOnFileWritting(_dataFolder, _filename);
        return;
    }

    emit informationMsg(QString("Snapshot dumped to %1").arg(_filename));
}

//------------------------------------------------------------------------------

void Core::requestAcousticWave(double frequency, double power)
{
    bool waveChanged = false;

    waveChanged |= _generator->setFrequency(frequency);
    waveChanged |= _driver->setPower(power);

    if (power != 0.0 && waveChanged)
    {
        qDebug("Acoustic wave stabilisation");
        _stabilisationT->start();
    } else {
        takeSnapshot();
    }
}

//------------------------------------------------------------------------------

void Core::setCommonParams(Mode mode,
                           int exposure,
                           int cooldownTime,
                           int cooldownPwr,
                           int stabilisationTime,
                           bool burst,
                           bool record,
                           const QString &dataFolder,
                           const QString &session)
{
    QByteArray s = session.toLatin1();

    qDebug("Exposure=%d ms, cooldownTime=%d ms, %d mW, "
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
    _record = record;
    _dataFolder = dataFolder;
    _session = session;
}

//------------------------------------------------------------------------------

}

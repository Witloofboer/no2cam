#include "Manager.h"

#include <QCoreApplication>
#include <QFile>
#include <QThread>
#include <QTimer>

#include "controllers.h"
#include "Crystal.h"
#include "ImageBuffer.h"
#include "modes.h"

//------------------------------------------------------------------------------

namespace core {

//------------------------------------------------------------------------------

Manager::Manager(const Crystal *crystal,
                 ThermometerDriver *thermometer,
                 CameraDriver *camera,
                 AcousticDriver *driver)
    : QObject()
    , _cooldownT(new QTimer(this))
    , _stabilisationT(new QTimer(this))
    , _crystal(crystal)
    , _cameraCtrl(new CameraCtrl(this, camera))
    , _acousticCtrl(new AcousticCtrl(this, driver))
    , _thermometerCtrl(new ThermometerCtrl(thermometer))
    , _snapshotBuffer{0}
    , _mode(0)
    , _thread(new QThread(this))
{
    _cooldownT->setSingleShot(true);
    _stabilisationT->setSingleShot(true);

    _cameraCtrl->setParent(this);
    _acousticCtrl->setParent(this);

    connect(_cooldownT, QTimer::timeout, this, onCooldownTimeout);
    connect(_stabilisationT, QTimer::timeout, this, onAcousticBeamReady);
    connect(this, updateTemperaturePeriod,
            _thermometerCtrl, ThermometerCtrl::onTemperaturePeriodUpdated);
    connect(this, shutdown,
            _thermometerCtrl, ThermometerCtrl::onShutdown);
    connect(_thermometerCtrl, ThermometerCtrl::updateTemperature,
            this, onTemperatureUpdated);
    connect(_cameraCtrl, CameraCtrl::snapshotAvailable,
            this, onSnapshotAvailable);
    connect(_thread, QThread::finished, this, onThreadFinished);
    qDebug("Moving core layer to core thread");
    moveToThread(_thread);

    qDebug("Starting core thread");
    _thread->start();

}

//------------------------------------------------------------------------------

Manager::~Manager()
{
    Q_ASSERT(_mode == nullptr);
    delete _thermometerCtrl;
}

//------------------------------------------------------------------------------

double Manager::temperature() const
{
    return _temperature;
}

//------------------------------------------------------------------------------

void Manager::setAcousticBeam(double frequency, double power)
{
    bool waveChanged = _acousticCtrl->set(frequency, power);

    if (waveChanged)
    {
        qDebug("Acoustic wave stabilisation: %d ms", _stabilisationT->interval());
        _stabilisationT->start();
    } else {
        onAcousticBeamReady();
    }
}

//------------------------------------------------------------------------------

void Manager::takeSnapshot()
{
    _cameraCtrl->takeSnapshot();
}

//------------------------------------------------------------------------------

void Manager::setSnapshotForGui(const Snapshot &snapshotBuffer)
{
    gImageBuffer.set(snapshotBuffer);
    emit displaySnapshot();
}

//------------------------------------------------------------------------------

void Manager::saveSnapshot(const QDateTime& dateTime,
                           char mode,
                           double wavelength,
                           double frequency,
                           double power,
                           int snapPerObs,
                           double temperature,
                           const Snapshot &snapshotBuffer)
{
    if (!_record) return;

    const QChar zero('0');

    QString expo = (mode=='O')
                   ? QString("%1x%2").arg(snapPerObs).arg(_exposure)
                   : QString("%1").arg(_exposure);

    auto _filename = QString("%1-%2-%3-%4nm-%5Ghz-%6mW-%7ms-%8degC.dat")
                     . arg(dateTime.toString("yyMMdd-HHmmss.zzz"))
                     . arg(_session.isEmpty() ? "_" : _session)
                     . arg(mode)
                     . arg(wavelength, 1, 'f', 1, zero)
                     . arg(frequency, 1, 'f', 3, zero)
                     . arg(power, 1, 'f', 0, zero)
                     . arg(expo)
                     . arg(temperature, 1, 'f', 1, zero);

    QFile file(_dataFolder+"/"+_filename);
    qDebug("Dumping snapshot to %s", _filename.toLatin1().constData());

    const bool ok = file.open(QIODevice::WriteOnly);

    if (! ok)
    {
        onStop();
        emit fileCreationError(_dataFolder, _filename);
        return;
    }

    auto buf = reinterpret_cast<const char*>(snapshotBuffer);
    const qint64 bufSize = snapshotSize*snapshotSize*2;

    const qint64 onDisk = file.write(buf, bufSize);

    file.close();

    if (bufSize != onDisk)
    {
        onStop();
        emit fileWritingError(_dataFolder, _filename);
        return;
    }

    emit displayInformation(QString("Snapshot dumped to %1").arg(_filename));
}

//------------------------------------------------------------------------------

void Manager::onStop()
{
    _acousticCtrl->set(0.0, 0.0);
    _cooldownT->stop();
    _stabilisationT->stop();
    _cameraCtrl->stop();
    delete _mode;
    _mode = nullptr;

    emit updateApplicationReadiness(true);
}

//------------------------------------------------------------------------------

void Manager::onOpticalSnapshot(double wavelength,
                                int exposure,
                                int cooldownTime,
                                int stabilisationTime,
                                bool burst,
                                bool record,
                                QString dataFolder,
                                QString session)
{
    Q_ASSERT(_mode == nullptr);

    qDebug("Spectral snap: wl=%.1f nm", wavelength);

    setParams(exposure, cooldownTime, stabilisationTime,
              burst, record, dataFolder, session);

    _mode = new OpticalSnapMode(*this, *_crystal, wavelength);
    _mode->start();
}

//------------------------------------------------------------------------------

void Manager::onAcousticSnapshot(double frequency,
                                 double power,
                                 int exposure,
                                 int cooldownTime,
                                 int stabilisationTime,
                                 bool burst,
                                 bool record,
                                 QString dataFolder,
                                 QString session)
{
    Q_ASSERT(_mode == nullptr);

    qDebug("Acoustic: freq=%.1f MHz, power=%.1f mW", frequency, power);

    setParams(exposure, cooldownTime, stabilisationTime,
              burst, record, dataFolder, session);

    _mode = new AcousticSnapMode(*this, *_crystal, frequency, power);
    _mode->start();
}

//------------------------------------------------------------------------------

void Manager::onObservation(double wavelength1,
                            double wavelength2,
                            int snapshotPerObs,
                            int exposure,
                            int cooldownTime,
                            int stabilisationTime,
                            bool burst,
                            bool record,
                            QString dataFolder,
                            QString session)
{
    Q_ASSERT(_mode == nullptr);

    qDebug("Observation: wl1=%.1f nm, wl2=%.1f nm",
           wavelength1, wavelength2);

    setParams(exposure, cooldownTime, stabilisationTime,
              burst, record, dataFolder, session);

    _mode = new ObservationMode(*this, *_crystal,
                                wavelength1, wavelength2, snapshotPerObs);
    _mode->start();
}

//------------------------------------------------------------------------------

void Manager::onSweep(double wavelength1,
                      double wavelength2,
                      double wavelengthStep,
                      int blackSnapshotRate,
                      int exposure,
                      int cooldownTime,
                      int stabilisationTime,
                      bool burst,
                      bool record,
                      QString dataFolder,
                      QString session)
{
    Q_ASSERT(_mode == nullptr);

    qDebug("Sweep: wl1=%.1f nm, wl2=%.1f nm, step=%.1f nm",
           wavelength1, wavelength2, wavelengthStep);

    setParams(exposure, cooldownTime, stabilisationTime,
              burst, record, dataFolder, session);

    _mode = new SweepMode(*this,
                          *_crystal,
                          wavelength1,
                          wavelength2,
                          wavelengthStep,
                          blackSnapshotRate);
    _mode->start();
}


//------------------------------------------------------------------------------

void Manager::onTemperatureUpdated(double temperature)
{
    _temperature=temperature;
    emit updateTemperature(_temperature);
}

//------------------------------------------------------------------------------

void Manager::onTemperaturePeriodUpdated(int period)
{
    emit updateTemperaturePeriod(period);
}

//------------------------------------------------------------------------------

void Manager::onShutdown()
{
    onStop();
    emit shutdown();
    _thermometerCtrl->thread()->wait();
    thread()->quit();
}

//------------------------------------------------------------------------------

void Manager::onThreadFinished()
{
    qDebug("Moving core layer back to main thread");

    moveToThread(QCoreApplication::instance()->thread());
}

//------------------------------------------------------------------------------

void Manager::onCooldownTimeout()
{
    if (_mode) _mode->setAcousticWave();
}

//------------------------------------------------------------------------------

void Manager::onAcousticBeamReady()
{
    if (_mode) _mode->acousticBeamReady();
}

//------------------------------------------------------------------------------

void Manager::onSnapshotAvailable(const Snapshot& buffer)
{
    if (_mode)
    {
        _mode->processSnapshot(buffer);

        if (_bursting || _mode->mustContinueAquisition())
        {
            if (_cooldownT->interval() != 0 && _mode->canCooldown())
            {
                _acousticCtrl->set(0.0, 0.0);
                qDebug("Cooling down: %d ms", _cooldownT->interval());
                _cooldownT->start();
            } else {
                _mode->setAcousticWave();
            }
        } else {
            onStop();
        }
    }
}

//------------------------------------------------------------------------------

void Manager::setParams(int exposure,
                        int cooldownTime,
                        int stabilisationTime,
                        bool bursting,
                        bool record,
                        const QString& dataFolder,
                        const QString& session)
{
    QByteArray s = session.toLatin1();
    qDebug("Exposure %d ms, cooldown %d ms, "
           "stabilisation %d ms, %s, %s, session '%s'",
           exposure, cooldownTime, stabilisationTime,
           bursting ? "bursting" : "singleshot",
           record ? "recording" : "not recording",
           s.data());

    _exposure = exposure;
    _cameraCtrl->setExposure(exposure);
    _cooldownT->setInterval(cooldownTime);
    _stabilisationT->setInterval(stabilisationTime);
    _bursting = bursting;
    _record = record;
    _dataFolder = dataFolder;
    _session = session;
}

//------------------------------------------------------------------------------

}

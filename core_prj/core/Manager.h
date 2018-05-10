#ifndef MANAGER_H
#define MANAGER_H

#include <QDateTime>
#include <QObject>

#include "core_global.h"
#include "Snapshot.h"
#include "Modes.h"

class QTimer;

//------------------------------------------------------------------------------

namespace core {

class BaseMode;
class Crystal;
class BaseCameraDriver;
class BaseAcousticDriver;
class BaseThermometerDriver;
class CameraCtrl;
class FrequencyCtrl;
class AcousticCtrl;
class ThermometerCtrl;

//------------------------------------------------------------------------------

/**
 * @brief The Core class is the entry point to the business layer.
 *
 * It is run by its own thread.
 */
class CORESHARED_EXPORT Manager
    : public QObject
    , public IModeToManager
{
    Q_OBJECT

public:
    Manager(const Crystal *crystal,
            BaseThermometerDriver *thermometer,
            BaseCameraDriver *camera,
            BaseAcousticDriver *driver);

    virtual ~Manager();

    // IModeToManager
    double temperature() const override;
    void setAcousticBeam(double frequency, double power) override;
    void takeSnapshot() override;
    void setSnapshotForGui(const Snapshot &snapshotBuffer) override;
    void saveSnapshot(const QDateTime &dateTime,
                      char mode,
                      double wavelength,
                      double frequency,
                      double power,
                      int snapPerObs,
                      double temperature,
                      const Snapshot &snapshotBuffer) override;
    void stop() override;

signals:
    void updateTemperature(double temperature);
    void updateTemperaturePeriod(int period);
    void updateApplicationReadiness(bool isReady);
    void displaySnapshot();
    void fileCreationError(QString dirname, QString filename);
    void fileWritingError(QString dirname, QString filename);
    void displayWarning(QString msg);
    void displayInformation(QString msg);
    void shutdown();

public slots:
    /**
     * Initiate a snapshoting session for a fixed wavelength.
     *
     * @param wavelength optical wavelength [nm]
     * @param exposure exposure time [ms]
     * @param cooldown cooldown time [ms]
     * @param stabilisationTime duration needed by electronic boards to relax to a new
     *        set-point.
     * @param burst single snapshot or burst mode flag
     * @param session name used to prepend record files. No recording if empty.
     */
    void onOpticalSnapshot(double wavelength,
                           int exposure,
                           int cooldownTime,
                           int stabilisationTime,
                           bool burst,
                           bool record,
                           QString dataFolder,
                           QString session);

    /**
     * Initiate a snapshoting session for a fixed frequency and power.
     *
     * @param wavelength optical wavelength [nm]
     * @param exposure exposure time [ms]
     * @param cooldown cooldown time [ms]
     * @param stabilisationTime duration needed by electronic boards to relax to a new
     *        set-point.
     * @param burst single snapshot or burst mode flag
     * @param session name used to prepend record files. No recording if empty.
     */
    void onAcousticSnapshot(double frequency,
                            double power,
                            int exposure,
                            int cooldownTime,
                            int stabilisationTime,
                            bool burst,
                            bool record,
                            QString dataFolder,
                            QString session);

    /**
     * Initiate an observation
     * @param wavelength1 first of the two wavelengths [nm]
     * @param wavelength2 second of the two wavelengths [nm]
     * @param exposure exposure time [s]
     * @param snapshotPerObs number of snapshots composing a single observation
     * @param cooldown cooldown time [s]
     * @param stabilisationTime duration needed by electronic boards to relax to a new
     *        set-point.
     * @param burst single snapshot or burst mode flag
     * @param session name used to prepend record files. No recording if empty.
     */
    void onObservation(double wavelength1,
                       double wavelength2,
                       int snapshotPerObs,
                       int exposure,
                       int cooldownTime,
                       int stabilisationTime,
                       bool burst,
                       bool record,
                       QString dataFolder,
                       QString session);


    /**
     * Initiate a DOAS session.
     * @param wavelengthFile file containing the wavelength
     * @param snapshotPerObs number of snapshots composing a single observation
     * @param exposure exposure time [s]
     * @param cooldown cooldown time [s]
     * @param burst single snapshot or burst mode flag
     * @param stabilisationTime duration needed by electronic boards to relax to a new
     *        set-point.
     * @param session name used to prepend record files. No recording if empty.
     * @param crystal crystal used
     */
    void onDoas(QString wavelengthFile,
                int snapshotPerObs,
                int exposure,
                int cooldownTime,
                int stabilisationTime,
                bool burst,
                bool record,
                QString dataFolder,
                QString session);

    /**
     * Initiate a sweep session.
     * @param wavelength1 initial wavelength [nm]
     * @param wavelength2 final wavelength [nm]
     * @param wavelengthStep wavelength step [nm]
     * @param exposure exposure time [s]
     * @param cooldown cooldown time [s]
     * @param burst single snapshot or burst mode flag
     * @param stabilisationTime duration needed by electronic boards to relax to a new
     *        set-point.
     * @param session name used to prepend record files. No recording if empty.
     * @param crystal crystal used
     */
    void onSweep(double wavelength1,
                 double wavelength2,
                 double wavelengthStep,
                 int    blackSnapshotRate,
                 int exposure,
                 int cooldownTime,
                 int stabilisationTime,
                 bool burst,
                 bool record,
                 QString dataFolder,
                 QString session);


    /**
      * Update the temperature
      */
    void onTemperatureUpdated(double temperature);

    /**
      * Update the temperature period
      */
    void onTemperaturePeriodUpdated(int period);


    /**
     * Requests the manager to shut down.
     */
    void onShutdown();

    /**
     * Requests the instance to move to the main thread.
     */
    void onThreadFinished();

private slots:
    void onCooldownTimeout();
    void onAcousticBeamReady();
    void onSnapshotAvailable(const Snapshot &buffer);

protected:
    void setParams(int exposure,
                   int cooldownTime,
                   int stabilisationTime,
                   bool bursting,
                   bool record,
                   const QString& dataFolder,
                   const QString& session);

    QTimer *_cooldownT;
    QTimer *_stabilisationT;

    const Crystal *_crystal;
    CameraCtrl *_cameraCtrl;
    AcousticCtrl *_acousticCtrl;
    ThermometerCtrl *_thermometerCtrl;

    double _temperature;

    int _exposure;
    bool _bursting;
    Snapshot _snapshotBuffer;

    bool _record;
    QString _dataFolder;
    QString _session;

    BaseMode *_mode;
    QThread *_thread;
};

//------------------------------------------------------------------------------

}

#endif // MANAGER_H

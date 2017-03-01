#ifndef MANAGER_H
#define MANAGER_H

#include <QDateTime>
#include <QObject>

#include "core_global.h"
#include "Snapshot.h"
#include "modes.h"

class QTimer;

//------------------------------------------------------------------------------

namespace core {

class BaseMode;
class Crystal;
class CameraDriver;
class AcousticDriver;
class ProbeDriver;
class CameraCtrl;
class FrequencyCtrl;
class AcousticCtrl;
class ProbeCtrl;

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
            ProbeDriver *probe,
            CameraDriver *camera,
            AcousticDriver *driver);

    ~Manager();

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
    void ready(bool isReady);
    void snapshotAvailableForGui();
    void errorOnFileCreation(QString dirname, QString filename);
    void errorOnFileWritting(QString dirname, QString filename);
    void temperatureUpdated(double temperature);
    void informationMsg(QString msg);

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
    void opticalSnapshot(double wavelength,
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
    void acousticSnapshot(double frequency,
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
    void observation(double wavelength1,
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
    void sweep(double wavelength1,
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
      * Update the temperature period
      */
    void updateTemperaturePeriod(int temperaturePeriod);

    /**
     * Requests the manager to shut down.
     */
    void shutdown();

    /**
     * Requests the instance to move to the main thread.
     */
    void threadFinished();

private slots:
    void cooledDowned();
    //void setAcousticWave();
    void acousticBeamReady();
    void updateTemperature();
    //void postSnapshotProcess();
    void snapshotReady();


private:
    void start();
    void setMode(BaseMode *mode);
    void setParams(int exposure,
                   int cooldownTime,
                   int stabilisationTime,
                   bool bursting,
                   bool record,
                   const QString& dataFolder,
                   const QString& session);

    enum Mode {READY, SpectralSnap, AcousticSnap, Obs, Sweep};
    const QString _modeToCode[5] = {"XX", "S", "A", "O", "W"};
    /*
        void setCommonParams(Mode mode,
                             int exposure,
                             int cooldownTime,
                             int cooldownPwr,
                             int stabilisationTime,
                             bool burst,
                             bool record,
                             const QString &dataFolder,
                             const QString &session);
    */
    QTimer *_cooldownT;
    QTimer *_stabilisationT;
    QTimer *_temperatureT;


//    double _cooldownPwr;

    const Crystal *_crystal;
    CameraCtrl *_camera;
    AcousticCtrl *_acousticDriver;
    ProbeCtrl *_probe;

    double _temperature;
//    double _refTemperature;

    //Mode _swicthMode;
    bool _isReady;
    int _exposure;
    bool _bursting;
    Snapshot _snapshotBuffer;
    // QDateTime _snapTime;
    bool _record;
    QString _dataFolder;
    QString _session;

    //bool mustContinueAquisition() const;
    //bool mustCooldown() const;

    /*
        struct WlSnapshotParams
        {
            double in_wavelength;
            double frequency;
            double power;
        };

        struct AcousticSnapshotParams
        {
            double in_frequency;
            double in_power;
            double wavelength;
        };

        struct ObservationParams
        {
            double in_wavelengths[3];
            int in_snapshotPerObs;
            int idx;
            int snapshotCount;
            double frequency[3];
            double power[3];
            Snapshot snapshots[3];
        };

        struct SweepParams
        {
            double in_minWavelength;
            double in_maxWavelength;
            double in_wavelengthStep;
            double wavelength;
            double frequency;
            double power;
            int blackSnapshotRate;
            int counter;
        };

        union {
            WlSnapshotParams specSnap;
            AcousticSnapshotParams acouSnap;
            ObservationParams obs;
            SweepParams swp;
        } _p; // parameters
    */
    BaseMode *_mode;
};

//------------------------------------------------------------------------------

}

#endif // MANAGER_H

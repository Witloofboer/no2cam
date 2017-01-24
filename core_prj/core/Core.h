#ifndef CORE_H
#define CORE_H

#include <QDateTime>
#include <QObject>

#include "core_global.h"
#include "BaseCamera.h"

class QTimer;

//------------------------------------------------------------------------------

namespace core {

class Crystal;
class AbstractCrysTempProbe;
class AbstractDriver;
class AbstractGenerator;

//------------------------------------------------------------------------------

/**
 * @brief The Core class is the entry point to the business layer.
 *
 * It is run by its own thread.
 */
class CORESHARED_EXPORT Core : public QObject
{
    Q_OBJECT

public:
    Core(const Crystal *crystal,
         AbstractCrysTempProbe *crysTempProb,
         BaseCamera *camera,
         AbstractGenerator *generator,
         AbstractDriver *driver); // Todo Timer factory

signals:
    void ready(bool isReady);
    void snapshotAvailable();

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
    void spectralSnapshot(double wavelength,
                          int exposure,
                          int cooldownTime,
                          int cooldownPwr,
                          int stabilisationTime,
                          bool burst,
                          bool record,
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
                          int cooldownPwr,
                          int stabilisationTime,
                          bool burst,
                          bool record,
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
                     int exposure,
                     int snapshotPerObs,
                     int cooldownTime,
                     int cooldownPwr,
                     int stabilisationTime,
                     bool burst,
                     bool record,
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
               int exposure,
               int cooldownTime,
               int cooldownPwr,
               int stabilisationTime,
               bool burst,
               bool record,
               QString session);

    /**
      * Requests the stop of all the devices.
      */
    void stop();

    /**
     * Requests the instance to move to the main thread.
     */
    void moveToMainThread();

    void postSnapshotProcess();

private slots:
    void setAcousticWave();
    void takeSnapshot();

private:
    enum Mode {READY, SpectralSnap, AcousticSnap, Obs, Sweep};
    const QString _modeToCode[5] = {"XX", "S", "A", "O", "W"};

    void cooldown();
    void setCommonParams(Mode mode,
                         int exposure,
                         int cooldownTime,
                         int cooldownPwr,
                         int stabilisationTime,
                         bool burst,
                         bool record,
                         const QString &session);

    void saveSnapshot(const QDateTime &dateTime,
                      Mode mode,
                      double wavelength,
                      double frequency,
                      double power,
                      int snapPerObs,
                      int exposure,
                      double temperature,
                      BaseCamera::Snapshot &snapshot);

    QTimer *_cooldownT;
    QTimer *_stabilisationT;


    double _cooldownPwr;

    const Crystal *_crystal;
    AbstractCrysTempProbe *_crysTempProb;
    BaseCamera *_camera;
    AbstractGenerator *_generator;
    AbstractDriver *_driver;

    Mode _mode;
    int _exposure;
    bool _bursting;
    BaseCamera::Snapshot _snapshot;
    QDateTime _snapTime;
    bool _record;
    QString _session;

    struct WlSnapshotParams
    {
        double in_wavelength;
        double temperature;
        double frequency;
        double power;
    };

    struct AcousticSnapshotParams
    {
        double in_frequency;
        double in_power;
        double temperature;
        double wavelength;
    };

    struct ObservationParams
    {
        double in_wavelengths[2];
        int in_snapshotPerObs;
        int idx;
        int snapshotCount;
        double temperature;
        double frequency[2];
        double power[2];
        BaseCamera::Snapshot snapshots[2];
    };

    struct SweepParams
    {
        double in_minWavelength;
        double in_maxWavelength;
        double in_wavelengthStep;
        double wavelength;
        double temperature;
        double frequency;
        double power;
    };

    union {
        WlSnapshotParams specSnap;
        AcousticSnapshotParams acouSnap;
        ObservationParams obs;
        SweepParams swp;
    } _p; // parameters

};

//------------------------------------------------------------------------------

}

#endif // CORE_H

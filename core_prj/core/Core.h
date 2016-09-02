#ifndef CORE_H
#define CORE_H

#include <QObject>

#include "core_global.h"

class QTimer;

//------------------------------------------------------------------------------

namespace core {

class AbstractCamera;
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
         AbstractCamera *camera,
         AbstractGenerator *generator,
         AbstractDriver *driver); // Todo Timer factory

signals:
    void ready(bool isReady);

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
                          const QString& session);

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
                          const QString& session);

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
                     const QString& session);


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
               const QString& session);

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

private:
    enum Mode {READY, SpSNAP, AcSNAP, OBS, SWP};

    void cooldown();
    void setCommonParams(Mode mode,
                         int exposure,
                         int cooldownTime,
                         int cooldownPwr,
                         int stabilisationTime,
                         bool burst,
                         const QString &session);

    void setOptimalAcousticWave(double wavelength);

    QTimer *_cooldownT;
    QTimer *_stabilisationT;

    double _cooldownPwr;

    const Crystal *_crystal;
    AbstractCrysTempProbe *_crysTempProb;
    AbstractCamera *_camera;
    AbstractGenerator *_generator;
    AbstractDriver *_driver;

    Mode _mode;
    bool _bursting;
    QString _session;

    struct WlSnapshotParams
    {
        double wavelength;
    };

    struct AcousticSnapshotParams
    {
        double frequency;
        double power;
    };

    struct ObservationParams
    {
        double wavelengths[2];
        int wavelengthIx;
        int snapshotCount;
        int snapshotPerObs;
    };

    struct SweepParams
    {
        double minWavelength;
        double maxWavelength;
        double wavelength;
        double wavelengthStep;
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

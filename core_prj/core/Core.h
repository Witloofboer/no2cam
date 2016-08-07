#ifndef CORE_H
#define CORE_H

#include <QObject>

#include "core_global.h"

//------------------------------------------------------------------------------

namespace core {

class Crystal;
class CoreThread;

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
    Core(const Crystal *crystal);

signals:
    void ready(bool isReady);

public slots:
    /**
     * Initiate a snapshoting session.
     *
     * @param wavelength optical wavelength [nm]
     * @param frequency acoustic frequency [MHz]
     * @param power acoustic power [mW]
     * @param exposure exposure time [s]
     * @param cooldown cooldown time [s]
     * @param burst single snapshot or burst mode flag
     * @param session name used to prepend record files. No recording if empty.
     * @param crystal crystal used
     */
    void startSnapshot(double wavelength,
                       double frequency,
                       double power,
                       double exposure,
                       double cooldown,
                       bool burst,
                       const QString& session);

    /**
     * Initiate an observation
     * @param wavelength1 first of the two wavelengths [nm]
     * @param wavelength2 second of the two wavelengths [nm]
     * @param exposure exposure time [s]
     * @param snapshotPerObs number of snapshots composing a single observation
     * @param cooldown cooldown time [s]
     * @param burst single snapshot or burst mode flag
     * @param session name used to prepend record files. No recording if empty.
     * @param crystal crystal used
     */
    void startObservation(double wavelength1,
                          double wavelength2,
                          double exposure,
                          int snapshotPerObs,
                          double cooldown,
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
     * @param session name used to prepend record files. No recording if empty.
     * @param crystal crystal used
     */
    void startSweep(double wavelength1,
                    double wavelength2,
                    double wavelengthStep,
                    double exposure,
                    double cooldown,
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


private:
    void doneImpl();
    const Crystal *_crystal;
};

//------------------------------------------------------------------------------

}

#endif // CORE_H

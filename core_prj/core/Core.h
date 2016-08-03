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

signals:
    /**
     * Emitted once the requested operation is done.
     */
    void done();

public slots:
    /**
     * Requests snapshotting
     * @param wavelength optical wavelength [nm]
     * @param frequency acoustic frequency [MHz]
     * @param power acoustic power [mW]
     * @param exposure exposure time [s]
     * @param cooldown cooldown time [s]
     * @param burst single snapshot or burst mode flag
     * @param session name used to prepend record files. No recording if empty.
     * @param crystal crystal used
     */
    void snapshot(double wavelength,
                  double frequency,
                  double power,
                  double exposure,
                  double cooldown,
                  bool burst,
                  const QString& session,
                  const Crystal& crystal);

    /**
      * Requests the stop of all the devices.
      */
    void stop();

    /**
     * Requests the instance to move to the main thread.
     */
    void moveToMainThread();

    void doneImpl();
};

//------------------------------------------------------------------------------

}

#endif // CORE_H

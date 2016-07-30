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

public slots:
    /**
      * Requests the stop of all the devices.
      */
    void stopDevices();

    /**
     * Requests the instance to move to the main thread.
     */
    void moveToMainThread();
};

//------------------------------------------------------------------------------

}

#endif // CORE_H

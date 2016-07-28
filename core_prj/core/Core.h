#ifndef CORE_H
#define CORE_H

#include <QObject>

#include "core_global.h"

//------------------------------------------------------------------------------

namespace core {

class Crystal;
class CrystalParameters;
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
    Core();
    virtual ~Core();

public slots:
    /**
     * Sets the parameters.
     */
    void setParameters(const CrystalParameters& params);

    /**
      * Requests the stop of all the devices.
      */
    void stopDevices();

    /**
     * Requests the instance to move to the main thread.
     */
    void moveToMainThread();

private:
    Crystal* crystal_;
};

//------------------------------------------------------------------------------

}

#endif // CORE_H

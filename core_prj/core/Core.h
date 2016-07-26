#ifndef CORE_H
#define CORE_H

#include <QObject>

#include "core_global.h"

class Crystal;
class CrystalParameters;
class CoreThread;

/**
 * @brief The Core class is the entry point to the business layer.
 *
 * It is run by its own thread.
 */
class CORESHARED_EXPORT Core : public QObject
{
    Q_OBJECT

private:
    Core();

public:
    virtual ~Core();

    /**
     * Starts the core layer thread and move the core singleton to it.
     */
    void start();

    /**
     * Wait that the core layer thread ends. This method has to be called from
     * the main thread. At the end of the execution the core singleton is moved
     * back to the main thread.
    */
    void wait();

    /**
     * Return the core singleton.
     */
    static Core *singleton();

public slots:

    /**
     * Set the parameters.
     */
    void setParameters(const CrystalParameters& params);

    /**
      * Requests the stop of all the devices.
      */
    void stopDevices();

    /**
     * Requests the shutdown of the core layer.
     */
    void shutdown();

private:
    Crystal* crystal_;
    CoreThread* thread_;
};

#endif // CORE_H

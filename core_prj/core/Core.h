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
     * Initialises the core layer.
     */
    static void init();


    /**
     * Starts the core layer thread.
     *
     * \note: the core singleton is moved to that thread.
     */
    static void start();


    /**
     * Shutdowns the core layer thread.
     *
     * \note:
     *     1. The core singleton is moved back to the main thread.
     *     2. The call waits for the end of the thread.
    */
    static void shutdown();


    /**
     * Initialises the core layer.
     */
    static void finalise();


    /**
     * Return the core singleton.
     */
    static Core *singleton();


public slots:
    /**
     * Sets the parameters.
     */
    void setParameters(const CrystalParameters& params);

    /**
      * Requests the stop of all the devices.
      */
    void stopDevices();

private slots:
    void shutdown_();

private:
    Crystal* crystal_;
};

#endif // CORE_H

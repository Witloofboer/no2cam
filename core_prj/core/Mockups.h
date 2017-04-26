#ifndef MOCKUPS_H
#define MOCKUPS_H

//------------------------------------------------------------------------------

#include <exception>

#include <QObject>

#include "core_global.h"
#include "BaseDrivers.h"

class QTimer;

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class CORESHARED_EXPORT MockAcousticDriver : public BaseAcousticDriver
{
    Q_OBJECT

public:
    explicit MockAcousticDriver();
    virtual void set(double frequency, double power) override;
};

//------------------------------------------------------------------------------

class CORESHARED_EXPORT MockThermometer : public BaseThermometerDriver
{
    Q_OBJECT

public:
    MockThermometer();
    double getTemperature() override;

private:
    double _temperature;
    double _delta;
};

//------------------------------------------------------------------------------

class CORESHARED_EXPORT MockCamera: public BaseCameraDriver
{
    Q_OBJECT

public:
    MockCamera();

    void setExposure(int exposure) override;
    void takeSnapshot() override;
    void stop() override;

private slots:
    void onSnapshotAvailable();

private:
    Snapshot _buffer;
    int _exposure;
    QTimer *_timer;
    Snapshot _scene;
    int _shift;
};

//------------------------------------------------------------------------------

}
#endif // MOCKUPS_H

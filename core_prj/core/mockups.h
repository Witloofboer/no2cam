#ifndef MOCKUPS_H
#define MOCKUPS_H

//------------------------------------------------------------------------------

#include <QObject>

#include "core_global.h"
#include "drivers.h"

class QTimer;

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class CORESHARED_EXPORT MockGenerator : public FrequencyDriver
{
    Q_OBJECT

public:
    explicit MockGenerator();
    void setFrequency(double frequency) override;
};

//------------------------------------------------------------------------------

class CORESHARED_EXPORT MockDriver : public PowerDriver
{
    Q_OBJECT

public:
    explicit MockDriver();
    virtual void setPower(double power) override;
};

//------------------------------------------------------------------------------

class CORESHARED_EXPORT MockProbe : public ProbeDriver
{
    Q_OBJECT

public:
    MockProbe();
    double getTemperature() override;

private:
    double _temperature;
    double _delta;
};

//------------------------------------------------------------------------------

class CORESHARED_EXPORT MockCamera: public CameraDriver
{
    Q_OBJECT

public:
    MockCamera();

    void setExposure(int exposure) override;
    void takeSnapshot() override;
    void stop() override;
    void getSnapshot(Snapshot &buffer) override;

private slots:
    void onSnapshotAvailable();

private:
    int _exposure;
    QTimer *_timer;
    Snapshot _scene;
    int _shift;
};

//------------------------------------------------------------------------------

}
#endif // MOCKUPS_H

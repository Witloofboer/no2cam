#ifndef CONTROLLERS_H
#define CONTROLLERS_H

//------------------------------------------------------------------------------

#include <QObject>

#include "snapshot.h"

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class FrequencyDriver;

class FrequencyCtrl : public QObject
{
    Q_OBJECT

public:
    explicit FrequencyCtrl(QObject *parent, FrequencyDriver *generator);
    bool setFrequency(double frequency);

private:
    FrequencyDriver *_generator;
    double _frequency;
};

//------------------------------------------------------------------------------

class PowerDriver;

class PowerCtrl : public QObject
{
    Q_OBJECT

public:
    explicit PowerCtrl(QObject *parent, PowerDriver *driver);
    bool setPower(double power);

private:
    PowerDriver *_driver;
    double _power;
};

//------------------------------------------------------------------------------

class ProbeDriver;

class ProbeCtrl : public QObject
{
    Q_OBJECT

public:
    explicit ProbeCtrl(QObject *parent, ProbeDriver *probe);
    double getTemperature();

protected:
    ProbeDriver *_probe;
};

//------------------------------------------------------------------------------

class CameraDriver;

class CameraCtrl : public QObject
{
    Q_OBJECT

public:
    explicit CameraCtrl(QObject *parent, CameraDriver *camera);

    void setExposure(int exposure);
    void takeSnapshot();
    void stop();
    void getSnapshot(Snapshot &buffer);

signals:
    void snapshotAvailable();

private slots:
    void processSnapshot();

private:
    CameraDriver *_camera;
    int _exposure;
    bool _isAvailable;
};

//------------------------------------------------------------------------------

}

#endif // CONTROLLERS_H

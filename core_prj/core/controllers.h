#ifndef CONTROLLERS_H
#define CONTROLLERS_H

//------------------------------------------------------------------------------

#include <QObject>

#include "snapshot.h"

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class AcousticDriver;

class AcousticCtrl : public QObject
{
    Q_OBJECT

public:
    explicit AcousticCtrl(QObject *parent, AcousticDriver *driver);
    bool set(double frequency, double power);

private:
    AcousticDriver *_driver;
    double _frequency;
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

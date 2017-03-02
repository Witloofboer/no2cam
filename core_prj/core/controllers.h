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

class ThermometerDriver;

class ThermometerCtrl : public QObject
{
    Q_OBJECT

public:
    explicit ThermometerCtrl(QObject *parent, ThermometerDriver *thermometer);
    double getTemperature();

protected:
    ThermometerDriver *_thermometer;
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

signals:
    void snapshotAvailable(const Snapshot &buffer);

private slots:
    void onSnapshotAvailable(const Snapshot &buffer);

private:
    CameraDriver *_camera;
    int _exposure;
    bool _isBusy;
};

//------------------------------------------------------------------------------

}

#endif // CONTROLLERS_H

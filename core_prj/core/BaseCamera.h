#ifndef BASECAMERA_H
#define BASECAMERA_H

#include <QObject>

#include "core_global.h"

class QTimer;

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class CORESHARED_EXPORT BaseCamera : public QObject
{
    Q_OBJECT

public:
    static const qint64 size = 2048;

    typedef quint16 Pixel;
    typedef Pixel Snapshot[size][size];

    explicit BaseCamera();

    void setExposure(int exposure);
    void takeSnapshot();
    void stop();
    void getSnapshot(Snapshot &buffer);

signals:
    void snapshotAvailable();

protected:
    void snapshotReady();

    virtual void takeSnapshotImpl()=0;
    virtual void stopImpl()=0;
    virtual void getSnapshotImpl(Snapshot &buffer)=0;

protected:
    int _exposure;

private slots:
    void setAvailable();

private:
    bool _isAvailable;
};

//------------------------------------------------------------------------------

class CORESHARED_EXPORT MockCamera: public BaseCamera
{
    Q_OBJECT

public:
    MockCamera();

protected:
    void takeSnapshotImpl() override;
    void stopImpl() override;
    void getSnapshotImpl(Snapshot &buffer) override;

private slots:
    void snapshotRdyImpl();

private:
    QTimer *_timer;
    Snapshot _scene;
    int _shift;
};

//------------------------------------------------------------------------------

}

#endif // BASECAMERA_H

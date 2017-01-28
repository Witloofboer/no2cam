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
    void copySnapshotToBuffer(Snapshot &buffer);

signals:
    void snapshotAvailable();

protected:
    void notBusy();
    virtual void takeSnapshotImpl()=0;
    virtual void stopImpl()=0;
    virtual void copySnapshotToBufferImpl(Snapshot &buffer)=0;

protected:
    int _exposure;

private:
    bool _isBusy;
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
    void copySnapshotToBufferImpl(Snapshot &buffer) override;

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

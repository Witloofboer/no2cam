#include "BaseCamera.h"

#include <QDir> // Q_INIT_RESOURCE
#include <QColor>
#include <QImage>
#include <QTimer>

//------------------------------------------------------------------------------

static inline void initResource() {
    Q_INIT_RESOURCE(scene);
}

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

BaseCamera::BaseCamera()
    : QObject()
    , _exposure(-1)
    , _isBusy(false)
{}

//------------------------------------------------------------------------------

void BaseCamera::setExposure(int exposure)
{
    Q_ASSERT(!_isBusy);
    qDebug("Camera: exposure time set to %d ms", exposure);
    _exposure = exposure;
}

//------------------------------------------------------------------------------

void BaseCamera::takeSnapshot()
{
    Q_ASSERT(!_isBusy);
    qDebug("Camera: taking snapshot");
    _isBusy = true;
    takeSnapshotImpl();
}

//------------------------------------------------------------------------------

void BaseCamera::stop()
{
    if (_isBusy)
    {
        qDebug("Camera: stop");
        stopImpl();
    }
}

//------------------------------------------------------------------------------

void BaseCamera::copySnapshotToBuffer(Snapshot &buffer)
{
    Q_ASSERT(!_isBusy);

    qDebug("Camera: Importing snapshot");
    copySnapshotToBufferImpl(buffer);
}

//------------------------------------------------------------------------------

void BaseCamera::notBusy()
{
    _isBusy = false;
}

//------------------------------------------------------------------------------

MockCamera::MockCamera()
    : BaseCamera()
    , _timer(new QTimer(this))
    , _shift(0)
{
    initResource();

    _timer->setSingleShot(true);
    connect(_timer, QTimer::timeout, this, snapshotRdyImpl);

    QImage image(":/scene.jpg");

    for (int i=0; i<size; ++i)
        for (int j=0; j<size; ++j)
        {
            _scene[i][j] = static_cast<quint16>(qGray(image.pixel(i, j))<<8);
        }
}

//------------------------------------------------------------------------------

void MockCamera::takeSnapshotImpl()
{
    _timer->start(_exposure);
}

//------------------------------------------------------------------------------

void MockCamera::stopImpl()
{
    _timer->stop();
    notBusy();
}

//------------------------------------------------------------------------------

void MockCamera::copySnapshotToBufferImpl(Snapshot &buffer)
{
    for (int i=0; i<size; ++i)
        for (int j=0; j<size; ++j)
        {
            double pix = (double(_exposure)*_scene[(i+_shift) % size][j]) / 50.0;
            buffer[i][j] = pix<65535 ? pix : 65535;
        }

    _shift = (_shift+7) % size;
}

//------------------------------------------------------------------------------

void MockCamera::snapshotRdyImpl()
{
    qDebug("Camera: snapshot ready");
    notBusy();
    emit snapshotAvailable();
}

//------------------------------------------------------------------------------

}

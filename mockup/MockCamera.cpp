#include "MockCamera.h"

#include <QColor>
#include <QImage>
#include <QTimer>

//------------------------------------------------------------------------------

MockCamera::MockCamera()
    : core::AbstractCamera()
    , _timer(new QTimer(this))
    , _shift(0)
{
    _timer->setSingleShot(true);
    connect(_timer, QTimer::timeout, this, snapshotRdyImpl);

    QImage image(":/scene.jpg");

    for (int i=0; i<core::snapSize; ++i)
        for (int j=0; j<core::snapSize; ++j)
        {
            _scene[i][j] = static_cast<quint16>(qGray(image.pixel(i, j))<<8);
        }
}
//------------------------------------------------------------------------------


void MockCamera::takeSnapshot()
{
    Q_ASSERT(0<_exposure);

    qInfo("Camera: snapshotting");

    _timer->start(_exposure);
}

//------------------------------------------------------------------------------

void MockCamera::stop()
{
    if (_timer->isActive())
    {
        qInfo("Forcing camera stop");
        _timer->stop();
    }
}

//------------------------------------------------------------------------------

void MockCamera::copySnapshot(core::Snapshot &buffer)
{
    for (int i=0; i<core::snapSize; ++i)
        for (int j=0; j<core::snapSize; ++j)
        {
            buffer[i][j] = (_exposure*_scene[(i+_shift) % core::snapSize][j]) / 50;
        }

    _shift = (_shift+10) % core::snapSize;
}


//------------------------------------------------------------------------------

void MockCamera::snapshotRdyImpl()
{
    qInfo("Camera: snapshot ready");
    emit snapshotAvailable();
}

//------------------------------------------------------------------------------

#include "MockCamera.h"

#include <QColor>
#include <QImage>
#include <QTimer>

//------------------------------------------------------------------------------

MockCamera::MockCamera()
    : core::BaseCamera()
    , _timer(new QTimer(this))
    , _shift(0)
{
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


void MockCamera::takeSnapshot()
{
    qDebug("Camera: snapshotting");

    _timer->start(_exposure);
}

//------------------------------------------------------------------------------

void MockCamera::stop()
{
    if (_timer->isActive())
    {
        qDebug("Camera stop");
        _timer->stop();
    }
}

//------------------------------------------------------------------------------

void MockCamera::copySnapshot(Snapshot &buffer)
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
    emit snapshotAvailable();
}

//------------------------------------------------------------------------------

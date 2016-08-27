#include "MockCamera.h"

#include <QColor>
#include <QImage>
#include <QTimer>

//------------------------------------------------------------------------------

MockCamera::MockCamera()
    : core::AbstractCamera()
    , snapshot{0}
    , _timer(new QTimer(this))
{
    _timer->setSingleShot(true);
    connect(_timer, QTimer::timeout, this, snapshotRdyImpl);

    QImage image(":/scene.jpg");

    for (int i=0; i<2048; ++i)
        for (int j=0; j<2048; ++j)
        {
            _scene[i][j] = qGray(image.pixel(i, j))/255.0;
        }
}

//------------------------------------------------------------------------------

void MockCamera::takeSnapshot()
{
    Q_ASSERT(0<_exposure);

    for (int i=0; i<2048; ++i)
        for (int j=0; j<2048; ++j)
        {
            snapshot[i][j] = (_exposure*_scene[i][j])/1000.0;
        }
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

void MockCamera::snapshotRdyImpl()
{
    emit snapshotAvailable();
}

//------------------------------------------------------------------------------

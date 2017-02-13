#include "mockups.h"

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

MockAcousticDriver::MockAcousticDriver(): AcousticDriver() {}

//------------------------------------------------------------------------------

void MockAcousticDriver::set(double frequency, double power)
{
    qDebug("<acoustic wave: %.3f MHz, %.1f mW>", frequency, power);
}

//------------------------------------------------------------------------------

MockProbe::MockProbe()
    : ProbeDriver()
    , _temperature(20.0)
    , _delta(0.12)
{}

//------------------------------------------------------------------------------

double MockProbe::getTemperature()
{
    _temperature += _delta;
    if (_temperature < 18.0) _delta = fabs(_delta);
    else if (22.0 < _temperature) _delta = -fabs(_delta);
    qDebug("<probe: %.2f degC>", _temperature);
    return _temperature;
}

//------------------------------------------------------------------------------

MockCamera::MockCamera()
    : CameraDriver()
    , _exposure(-1)
    , _timer(new QTimer(this))
    , _shift(0)
{
    initResource();

    _timer->setSingleShot(true);
    connect(_timer, QTimer::timeout, this, onSnapshotAvailable);

    QImage image(":/scene.jpg");

    for (int i=0; i<snapshotSize; ++i)
        for (int j=0; j<snapshotSize; ++j)
        {
            _scene[i][j] = static_cast<quint16>(qGray(image.pixel(i, j))<<8);
        }
}

//------------------------------------------------------------------------------

void core::MockCamera::setExposure(int exposure)
{
    qDebug("<camera: %d ms>", exposure);
    _exposure = exposure;
}

//------------------------------------------------------------------------------

void MockCamera::takeSnapshot()
{
    qDebug("<camera: snapshotting>");
    _timer->start(_exposure);
}

//------------------------------------------------------------------------------

void MockCamera::stop()
{
    qDebug("<camera: stopped>");
    _timer->stop();
}

//------------------------------------------------------------------------------

void MockCamera::onSnapshotAvailable()
{
    qDebug("<camera: snapshot available>");
    emit snapshotAvailable();
}

//------------------------------------------------------------------------------

void MockCamera::getSnapshot(Snapshot &buffer)
{
    qDebug("<camera: transmitting snapshot>");
    for (int i=0; i<snapshotSize; ++i)
        for (int j=0; j<snapshotSize; ++j)
        {
            double pix = (double(_exposure)*_scene[(i+_shift) % snapshotSize][j]) / 50.0;
            buffer[i][j] = pix<65535 ? pix : 65535;
        }

    _shift = (_shift+7) % snapshotSize;
}



//------------------------------------------------------------------------------

}

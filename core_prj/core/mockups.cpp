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

MockGenerator::MockGenerator(): FrequencyDriver() {}

//------------------------------------------------------------------------------

void MockGenerator::setFrequency(double) {}

//------------------------------------------------------------------------------

MockDriver::MockDriver(): PowerDriver() {}

//------------------------------------------------------------------------------

void MockDriver::setPower(double) {}

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
    connect(_timer, QTimer::timeout, this, snapshotAvailable);

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
    _exposure = exposure;
}

//------------------------------------------------------------------------------

void MockCamera::takeSnapshot()
{
    _timer->start(_exposure);
}

//------------------------------------------------------------------------------

void MockCamera::stop()
{
    _timer->stop();
}

//------------------------------------------------------------------------------

void MockCamera::getSnapshot(Snapshot &buffer)
{
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

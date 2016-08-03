#include "Core.h"

#include <QCoreApplication>
#include <QTimer>

#include "Crystal.h"

//------------------------------------------------------------------------------

namespace core {

//------------------------------------------------------------------------------

void Core::snapshot(double wavelength, double frequency, double power,
                    double exposure, double cooldown, bool burst,
                    const QString& session, const Crystal& crystal)
{
    Q_UNUSED(crystal);

    QByteArray s = session.toLatin1();

    qInfo("Snapshot: wl=%.1f nm, fq=%.1f MHz, pwr=%.1f mW, expo=%.1f ms, "
          "cooldown=%.1f ms, %s, session='%s'",
          wavelength, frequency, power, exposure, cooldown,
          burst ? "burst" : "singleshot", s.data());

    if (!burst)
        QTimer::singleShot(exposure, this, doneImpl);
}

//------------------------------------------------------------------------------

void Core::stop()
{
    qInfo("Stopping devices");
    emit done(); // todo
}

//------------------------------------------------------------------------------

void Core::moveToMainThread()
{
    qInfo("Moving core singleton to main thread");
    moveToThread(QCoreApplication::instance()->thread());
}

//------------------------------------------------------------------------------

void Core::doneImpl()
{
    stop();
    emit done();
}

//------------------------------------------------------------------------------

}

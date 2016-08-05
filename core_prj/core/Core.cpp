#include "Core.h"

#include <QCoreApplication>
#include <QTimer>

#include "Crystal.h"

//------------------------------------------------------------------------------

namespace core {

//------------------------------------------------------------------------------

void Core::startSnapshot(double wavelength,
                         double frequency,
                         double power,
                         double exposure,
                         double cooldown,
                         bool burst,
                         const QString& session,
                         const Crystal& crystal)
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

void Core::startObservation(double wavelength1,
                            double wavelength2,
                            double exposure,
                            int snapshotPerObs,
                            double cooldown,
                            bool burst,
                            const QString &session,
                            const Crystal &crystal)
{
    Q_UNUSED(crystal);

    QByteArray s = session.toLatin1();

    qInfo("Observation: wl1=%.1f nm, wl2=%.1f nm, expo=%.1f ms, s/obs=%d, "
          "cooldown=%.1f ms, %s, session='%s'",
          wavelength1, wavelength2, exposure, snapshotPerObs, cooldown,
          burst ? "burst" : "singleshot", s.data());

    if (!burst)
        QTimer::singleShot(exposure, this, doneImpl);
}

void Core::startSweep(double wavelength1,
                      double wavelength2,
                      double wavelengthStep,
                      double exposure,
                      double cooldown,
                      bool burst,
                      const QString &session,
                      const Crystal &crystal)
{
    Q_UNUSED(crystal);

    QByteArray s = session.toLatin1();

    qInfo("Sweep: wl1=%.1f nm, wl2=%.1f nm, step=%.1f nm, expo=%.1f ms, "
          "cooldown=%.1f ms, %s, session='%s'",
          wavelength1, wavelength2, wavelengthStep, exposure, cooldown,
          burst ? "burst" : "singleshot", s.data());

    if (!burst)
        QTimer::singleShot(exposure, this, doneImpl);
}

//------------------------------------------------------------------------------

void Core::stopRequested()
{
    qInfo("Stopping devices");
    emit ready(); // todo
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
    stopRequested();
    emit ready();
}

//------------------------------------------------------------------------------

}

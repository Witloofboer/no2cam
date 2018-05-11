#ifndef INSTRUMENTEDMANAGER_H
#define INSTRUMENTEDMANAGER_H

#include "core/Manager.h"
#include <QElapsedTimer>
#include "core/Snapshot.h"

namespace gui
{
class MainWindow;
}


class InstrumentedManager : public core::Manager
{
public:
    InstrumentedManager(const core::Crystal *crystal,
                        core::BaseThermometerDriver *thermometer,
                        core::BaseCameraDriver *camera,
                        core::BaseAcousticDriver *driver);

    void mainWindow(gui::MainWindow* mainWindow);

    void saveSnapshot(const QDateTime &dateTime,
                      char mode,
                      int exposure,
                      double wavelength,
                      double frequency,
                      double power,
                      int snapPerObs,
                      double temperature,
                      const core::Snapshot &snapshotBuffer) override;
public slots:
    void runScenario();

private:
    QElapsedTimer _chrono;
    int _counter;
    core::Snapshot _buffer;
    gui::MainWindow* _mainWindow;
};

#endif // INSTRUMENTEDMANAGER_H

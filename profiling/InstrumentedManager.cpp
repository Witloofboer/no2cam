#include "InstrumentedManager.h"
#include <QtGlobal>
#include <cstring>
#include <QApplication>

#include "gui/MainWindow.h"

using namespace core;

InstrumentedManager::InstrumentedManager(const Crystal *crystal,
        BaseThermometerDriver *thermometer,
        BaseCameraDriver *camera,
        BaseAcousticDriver *driver)

    : Manager(crystal, thermometer, camera, driver)
    , _chrono()
    , _counter(0)
{
}

void InstrumentedManager::mainWindow(gui::MainWindow *mainWindow)
{
    _mainWindow = mainWindow;
}

void InstrumentedManager::runScenario()
{
    qInfo("Running scenario");
    _counter=0;
    _chrono.start();
    //onOpticalSnapshot(500, 0, 0, 0, true, false, "h:/cam_data", "test");
    onObservation(450, 550, 4, 0, 0, 0, true, false, "h:/cam_data", "test");
}

void InstrumentedManager::saveSnapshot(const QDateTime& dateTime,
                                       char mode,
                                       double wavelength,
                                       double frequency,
                                       double power,
                                       int snapPerObs,
                                       double temperature,
                                       const Snapshot &snapshotBuffer)
{
    Manager::saveSnapshot(dateTime, mode,
                          wavelength, frequency, power, snapPerObs,
                          temperature, snapshotBuffer);
    ++_counter;
    if (_counter==600)
    {
        qInfo("Result: %.3f ms/snap", ((double)_chrono.elapsed())/600);
        _bursting = false;
        _mainWindow->close();
    }
}

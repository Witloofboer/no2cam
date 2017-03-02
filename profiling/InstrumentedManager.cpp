#include "InstrumentedManager.h"
#include <QtGlobal>
#include <cstring>

using namespace core;

InstrumentedManager::InstrumentedManager(const Crystal *crystal,
        ThermometerDriver *thermometer,
        CameraDriver *camera,
        AcousticDriver *driver)

    : Manager(crystal, thermometer, camera, driver)
    , _chrono()
    , _counter(0)
{
}

void InstrumentedManager::runScenario()
{
    qInfo("Running scenario");
    _counter=0;
    _chrono.start();
    onOpticalSnapshot(500, 0, 0, 0, true, true, "h:/cam_data", "test");
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

    std::memcpy(_buffer, snapshotBuffer, sizeof _buffer);

    ++_counter;
    if (_counter==10)
    {
        qInfo("Result: %.3f ms/snap", ((double)_chrono.elapsed())/10);
        _bursting = false;
    }
}

#include <QApplication>

#include <QMessageBox>
#include "Camera.h"
#include "Driver.h"
#include "Thermometer.h"

#include <QCoreApplication>
#include "gui/MainWindow.h"
#include "core/Crystal.h"
#include "core/Manager.h"
#include "core/drivers.h"

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{

    qSetMessagePattern("%{time hh:mm:ss.zzz} (%{threadid}) %{type}: %{message}");

    QApplication application(argc, argv);

    auto thermometer = new Thermometer;     // comment for fake Probe
    //auto thermometer = new core::MockThermometer;     // uncomment for fake Probe
    auto driver = new Driver;
    auto camera = new HamamatsuCamera();    // comment for fake camera
    //auto camera = new core::MockCamera(); // uncomment for fake camera

    bool ok = camera->init();               // comment for fake camera
    if (!ok) return -1;                     // comment for fake camera

    bool thermometerOK = thermometer->init();           // comment for fake Probe
    if (!thermometerOK)                           // comment 'if structure' for fake Probe
    {
        QMessageBox::critical(0, "Aborting", "Failed to connect to Temperature Sensor.");
        return -1;
    }

    bool driverOK = driver->init();
    if (!driverOK)
    {
        QMessageBox::critical(0, "Aborting", "Failed to connect to RF GENERATOR module.");
        return -1;
    }

    qInfo("Initialisation");

    QCoreApplication::setOrganizationName("BIRA-IASB");
    QCoreApplication::setApplicationName("NO2 Camera Command Interface");

    auto _crystal = new core::Crystal;
    auto _coreLayer = new core::Manager(_crystal, thermometer, camera, driver);
    auto _mainWindow = new gui::MainWindow(_crystal, _coreLayer, "",
                                           QObject::tr("<p>Actual devices are used.</p>"));
    _mainWindow->show();

    int result = application.exec();

    camera->uninit();                      // comment for fake camera
    thermometer-> uninit();                      // comment for fake Probe
    return result;
}

//------------------------------------------------------------------------------

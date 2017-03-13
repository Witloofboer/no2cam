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

    QCoreApplication::setOrganizationName("BIRA-IASB");
    QCoreApplication::setApplicationName("NO2 Camera Command Interface");
    QApplication application(argc, argv);

    auto camera = new HamamatsuCamera();
    bool ok = camera->init();
    if (!ok) return -1;

    // Thermometer -------------------------------------------------------------
    auto thermometer = new Thermometer;
    bool ok = thermometer->init();
    if (!ok)
    {
        delete camera;

        QMessageBox::critical(0, "Aborting",
                              "Failed to connect to temperature sensor.");
        return -1;
    }

    // Electronic driver--------------------------------------------------------
    auto driver = new Driver;
    ok = driver->init();
    if (!ok)
    {
        delete camera;
        delete thermometer;

        QMessageBox::critical(0, "Aborting",
                              "Failed to connect to RF module.");
        return -1;
    }

    // -------------------------------------------------------------------------

    auto crystal = new core::Crystal;
    auto core = new core::Manager(crystal, thermometer, camera, driver);
    auto mainWindow = new gui::MainWindow(crystal, core, "",
                                          QObject::tr("<p>Actual devices are used.</p>"));
    mainWindow->show();

    int result = application.exec();

    return result;
}

//------------------------------------------------------------------------------

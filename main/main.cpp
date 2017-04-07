#include <QApplication>

#include <QMessageBox>
#include "Camera.h"
#include "Drivers.h"
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

    auto camera = HamamatsuCamera::getCamera();
    auto thermometer = Thermometer::getThermometer();
    auto driver = BaseDriver::getDriver();

    if (camera == nullptr || thermometer == nullptr || driver == nullptr)
    {
        delete camera;
        delete thermometer;
        delete driver;

        return -1;
    }

    // Crystal -----------------------------------------------------------------

    auto crystal = new core::Crystal;

    // Core layer --------------------------------------------------------------

    auto coreManager = new core::Manager(crystal, thermometer, camera, driver);

    // Main window -------------------------------------------------------------

    auto mainWindow
        = new gui::MainWindow(crystal, coreManager, "",
                              QObject::tr("<p>Actual devices are used.</p>"));
    mainWindow->show();

    int result = application.exec();

    delete mainWindow;
    delete coreManager;
    delete crystal;

    return result;
}

//------------------------------------------------------------------------------

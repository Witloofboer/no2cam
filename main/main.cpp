#include <QApplication>

#include <QMessageBox>
#include "Camera.h"
#include "Drivers.h"
#include "Thermometer.h"

#include <cstring>

#include <QCoreApplication>
#include "gui/MainWindow.h"
#include "core/Crystal.h"
#include "core/Manager.h"
#include "core/BaseDrivers.h"

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss.zzz} (%{threadid}) %{type}: %{message}");

    QCoreApplication::setOrganizationName("BIRA-IASB");
    QCoreApplication::setApplicationName("NO2 Camera Command Interface");
    QApplication application(argc, argv);

    bool isDds = false;
    bool isPll = false;

    if (argc == 2)
    {
        isDds = 0 == strcmp("-dds", argv[1]);
        isPll = 0 == strcmp("-pll", argv[1]);
    }

    BaseDriver *driver = 0;

    if (isDds)
    {
        driver = DdsDriver::getDriver();
    }
    else if (isPll)
    {
        driver = PllDriver::getDriver();
    }
    else
    {
        qCritical("Missing '-dll' or '-pll' parameter at launch");
        QMessageBox::critical(
            0,
            "Aborting",
            "<p><b>Failure</b>: "
            "The application requires either '-dll' or '-pll' as a parameter."
            "</p>");
        return -1;
    }

    auto camera = HamamatsuCamera::getCamera();
    auto thermometer = Thermometer::getThermometer();

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

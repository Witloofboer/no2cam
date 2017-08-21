#include <QApplication>

#include "core/Mockups.h"
#include "core/Crystal.h"
#include "core/Manager.h"

#include "gui/MainWindow.h"


//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss.zzz} (%{threadid}) %{type}: %{message}");

    QApplication application(argc, argv);

    auto thermometer = new core::MockThermometer(0.12);
    auto camera = new core::MockCamera;
    auto driver = new core::MockAcousticDriver;

    QCoreApplication::setOrganizationName("BIRA-IASB");
    QCoreApplication::setApplicationName("NO2 Camera Command Interface");

    auto _crystal = new core::Crystal;
    auto _coreLayer = new core::Manager(_crystal, thermometer, camera, driver);
    auto _mainWindow = new gui::MainWindow(_crystal, _coreLayer, "mockup",
                                           QObject::tr("<p>Functional mockups are used for all devices.</p>"));
    _mainWindow->show();

    int result = application.exec();

    delete _mainWindow;
    delete _coreLayer;
    delete _crystal;

    return result;
}

//------------------------------------------------------------------------------

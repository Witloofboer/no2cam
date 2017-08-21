#include <QApplication>


#include <QDebug>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QThread>
#include <QTimer>

#include "gui/MainWindow.h"
#include "core/Mockups.h"
#include "core/Crystal.h"
#include "core/Manager.h"
#include "core/BaseDrivers.h"

#include "InstrumentedManager.h"

//------------------------------------------------------------------------------


int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss.zzz} (%{threadid}) %{type}: %{message}");

    QApplication application(argc, argv);

    //QElapsedTimer chrono;
    //chrono.start();
    //qint64 elapsed1 = chrono.nsecsElapsed();

    auto thermometer = new core::MockThermometer(0.0);
    auto camera = new core::MockCamera;
    auto driver = new core::MockAcousticDriver;

    qInfo("Initialisation");
    QCoreApplication::setOrganizationName("BIRA-IASB");
    QCoreApplication::setApplicationName("NO2 Camera Command Interface");
    Q_INIT_RESOURCE(resources);

    auto _crystal = new core::Crystal;
    auto _coreLayer = new InstrumentedManager(_crystal, thermometer, camera, driver);
    auto _mainWindow = new gui::MainWindow(_crystal, _coreLayer, "(optim)", "");
    _mainWindow->show();

    _coreLayer->mainWindow(_mainWindow);

    QTimer::singleShot(2000, _coreLayer, InstrumentedManager::runScenario);
    qDebug("Starting Gui");
    int result = application.exec();
    qInfo("Finalisation");

    delete _mainWindow;
    delete _coreLayer;
    delete _crystal;
    return result;
}

//------------------------------------------------------------------------------

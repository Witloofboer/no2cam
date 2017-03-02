#include <QApplication>


#include <QDebug>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QThread>
#include <QTimer>

#include "gui/MainWindow.h"
#include "core/mockups.h"
#include "core/Crystal.h"
#include "core/Manager.h"
#include "core/drivers.h"

#include "InstrumentedManager.h"

//------------------------------------------------------------------------------


int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss.zzz} (%{threadid}) %{type}: %{message}");

    QApplication application(argc, argv);

    //QElapsedTimer chrono;
    //chrono.start();
    //qint64 elapsed1 = chrono.nsecsElapsed();

    auto thermometer = new core::MockThermometer;
    auto camera = new core::MockCamera;
    auto driver = new core::MockAcousticDriver;

    qInfo("Initialisation");
    QCoreApplication::setOrganizationName("BIRA-IASB");
    QCoreApplication::setApplicationName("NO2 Camera Command Interface");
    Q_INIT_RESOURCE(resources);

    auto _coreThr = new QThread;
    auto _crystal = new core::Crystal;
    auto _coreLayer = new InstrumentedManager(_crystal, thermometer, camera, driver);
    auto _mainWindow = new gui::MainWindow(_crystal, _coreLayer, "(optim)", "");
    _mainWindow->show();

    QObject::connect(_coreThr, QThread::finished,
                     _coreLayer, core::Manager::onThreadFinished);

    qDebug("Moving core layer to core thread");
    _coreLayer->moveToThread(_coreThr);

    qDebug("Starting core thread");
    _coreThr->start();

    QTimer::singleShot(2000, _coreLayer, InstrumentedManager::runScenario);
    qDebug("Starting Gui");
    int result = application.exec();
    qInfo("Finalisation");

    delete _mainWindow;
    delete _coreLayer;
    delete _coreThr;
    delete _crystal;
    return result;
}

//------------------------------------------------------------------------------

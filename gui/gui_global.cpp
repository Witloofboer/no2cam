#include "gui_global.h"

#include <QCoreApplication>
#include <QThread>

#include "MainWindow.h"
#include "core/Crystal.h"
#include "core/Manager.h"
#include "core/drivers.h"

//------------------------------------------------------------------------------

static QThread *_coreThr = nullptr;
static core::Crystal *_crystal = nullptr;
static core::Manager *_coreLayer = nullptr;
static gui::MainWindow *_mainWindow = nullptr;


//------------------------------------------------------------------------------

void init(const QString& subversion,
          const QString& devicesNotes,
          core::ThermometerDriver *thermometer,
          core::CameraDriver *camera,
          core::AcousticDriver *driver)
{
    qInfo("Initialisation");

    QCoreApplication::setOrganizationName("BIRA-IASB");
    QCoreApplication::setApplicationName("NO2 Camera Command Interface");
    Q_INIT_RESOURCE(resources);

    _coreThr = new QThread;
    _crystal = new core::Crystal;
    _coreLayer = new core::Manager(_crystal, thermometer, camera, driver);
    _mainWindow = new gui::MainWindow(_crystal, _coreLayer, subversion, devicesNotes);
    _mainWindow->show();

    QObject::connect(_coreThr, QThread::finished,
                     _coreLayer, core::Manager::onThreadFinished);
}

//------------------------------------------------------------------------------

void start()
{
    qDebug("Moving core layer to core thread");
    _coreLayer->moveToThread(_coreThr);

    qDebug("Starting core thread");
    _coreThr->start();
}

//------------------------------------------------------------------------------

void finalise()
{
    qInfo("Finalisation");

    delete _mainWindow;
    _mainWindow = nullptr;

    delete _coreLayer;
    delete _coreThr;
    delete _crystal;

    _coreLayer = nullptr;
    _coreThr = nullptr;
    _crystal = nullptr;
}

//------------------------------------------------------------------------------

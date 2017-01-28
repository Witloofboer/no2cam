#include "gui_global.h"

#include <QCoreApplication>
#include <QThread>

#include "MainWindow.h"
#include "core/Crystal.h"
#include "core/Core.h"
#include "core/drivers.h"

//------------------------------------------------------------------------------

using namespace gui;
using namespace core;

static QThread *_coreThr = nullptr;
static Crystal *_crystal = nullptr;
static Core *_coreLayer = nullptr;
static MainWindow *_mainWindow = nullptr;


//------------------------------------------------------------------------------

void init(const QString& subversion,
          const QString& devicesNotes,
          core::ProbeDriver *probe,
          core::CameraDriver *camera,
          core::FrequencyDriver *generator,
          core::PowerDriver *driver)
{
    qInfo("Initialisation");

    QCoreApplication::setOrganizationName("BIRA-IASB");
    QCoreApplication::setApplicationName("NO2 Camera Command Interface");
    Q_INIT_RESOURCE(resources);

    _coreThr = new QThread;
    _crystal = new Crystal;
    _coreLayer = new Core(_crystal, probe, camera, generator, driver);
    _mainWindow = new gui::MainWindow(_crystal, _coreLayer, subversion, devicesNotes);
    _mainWindow->show();

    QObject::connect(_coreThr, QThread::finished,
                     _coreLayer, Core::threadFinished);
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

void shutdown()
{
    qDebug("Shutting down core thread");
    _coreThr->exit();
    _coreThr->wait();
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

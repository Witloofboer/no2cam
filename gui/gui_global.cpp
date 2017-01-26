#include "gui_global.h"

#include <QCoreApplication>
#include <QThread>

#include "MainWindow.h"
#include "core/Crystal.h"
#include "core/Core.h"
#include "core/BaseCamera.h"

//------------------------------------------------------------------------------

using namespace gui;
using namespace core;

static QThread *_coreThr = nullptr;
static Crystal *_crystal = nullptr;
static Core *_coreLayer = nullptr;
static MainWindow *_mainWindow = nullptr;


//------------------------------------------------------------------------------

void init(const QString& version,
          const QString& releaseNotes,
          core::AbstractCrysTempProbe *crysTempProb,
          core::BaseCamera *camera,
          core::AbstractGenerator *generator,
          core::AbstractDriver *driver)
{
    qInfo("Initialisation");

    QCoreApplication::setOrganizationName("BIRA-IASB");
    QCoreApplication::setApplicationName("NO2 Camera Command Interface");
    Q_INIT_RESOURCE(resources);

    _coreThr = new QThread;
    _crystal = new Crystal;
    _coreLayer = new Core(_crystal, crysTempProb, camera, generator, driver);
    _mainWindow = new gui::MainWindow(_crystal, _coreLayer, version, releaseNotes);
    _mainWindow->show();

    QObject::connect(_coreThr, QThread::finished,
                     _coreLayer, Core::threadFinished);
}

//------------------------------------------------------------------------------

void start()
{
    qInfo("Moving core layer to core thread");
    _coreLayer->moveToThread(_coreThr);

    qInfo("Starting core thread");
    _coreThr->start();
}

//------------------------------------------------------------------------------

void shutdown()
{
    qInfo("Shutting down core thread");
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

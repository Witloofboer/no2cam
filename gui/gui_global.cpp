#include "gui_global.h"

#include <QCoreApplication>
#include <QThread>

#include "MainWindow.h"
#include "../core/Core.h"

//------------------------------------------------------------------------------

using namespace gui;
using namespace core;

static QThread *coreThr_ = nullptr;
static Core *coreLayer_ = nullptr;
static MainWindow *mainWindow_ = nullptr;


//------------------------------------------------------------------------------

void init(const char *version)
{
    qInfo("Initialisation");

    QCoreApplication::setOrganizationName("BIRA-IASB");
    QCoreApplication::setApplicationName("NO2 Camera Command Interface");
    Q_INIT_RESOURCE(resources);

    coreThr_ = new QThread;
    coreLayer_ = new Core;
    mainWindow_ = new gui::MainWindow(coreLayer_, version);
    mainWindow_->show();

    QObject::connect(coreThr_, QThread::finished,
                     coreLayer_, Core::moveToMainThread);
}

//------------------------------------------------------------------------------

void start()
{
    qInfo("Moving core layer to core thread");
    coreLayer_->moveToThread(coreThr_);

    qInfo("Starting core thread");
    coreThr_->start();
}

//------------------------------------------------------------------------------

void shutdown()
{
    qInfo("Shutting down core thread");
    coreThr_->exit();
    coreThr_->wait();
}

//------------------------------------------------------------------------------

void finalise()
{
    qInfo("Finalisation");

    delete mainWindow_;
    mainWindow_ = nullptr;

    delete coreLayer_;
    delete coreThr_;

    coreLayer_ = nullptr;
    coreThr_ = nullptr;

}

//------------------------------------------------------------------------------

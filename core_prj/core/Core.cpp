#include <QCoreApplication>
#include <QThread>

#include "Core.h"
#include "CoreThread.h"
#include "Crystal.h"

Core::Core()
    : QObject()
    , crystal_(new Crystal(this))
    , thread_(new CoreThread)
{
}

Core::~Core()
{
    delete crystal_;
    delete thread_;
}

void Core::start()
{
    qDebug("Starting core thread");
    thread_->start();
}

void Core::wait()
{
    qDebug("Waiting core thread");
    thread_->wait();
}

Core *Core::singleton()
{
    static Core *core = new Core();

    return core;
}


void Core::shutdown()
{
    qDebug("Exiting core thread");
    thread_->exit();
}


void Core::setParameters(const CrystalParameters &params)
{
    crystal_->setParameters(params);
}

void Core::stopDevices()
{
    qDebug("Stopping devices");
}

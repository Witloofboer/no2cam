#include "core_lib.h"

#include <QCoreApplication>
#include <QThread>

#include "Core.h"

//------------------------------------------------------------------------------

namespace core {

static QThread *thread_ = nullptr;
static Core *singleton_ = nullptr;

//------------------------------------------------------------------------------

void init()
{
    qDebug("Initialising core layer");
    thread_ = new QThread;
    singleton_ = new Core;

    QObject::connect(thread_, QThread::finished, singleton_, Core::moveToMainThread);
}

//------------------------------------------------------------------------------

void start()
{
    qDebug("Moving core singleton to core thread");
    singleton_->moveToThread(thread_);

    qDebug("Starting core thread");
    thread_->start();
}

//------------------------------------------------------------------------------

void shutdown()
{
    qDebug("Shutting down core thread");
    thread_->exit();
    thread_->wait();
}

//------------------------------------------------------------------------------

Core *singleton()
{
    return singleton_;
}

//------------------------------------------------------------------------------

void finalise()
{
    qDebug("Finalising core layer");
    delete singleton_;
    delete thread_;

    singleton_ = nullptr;
    thread_ = nullptr;
}

//------------------------------------------------------------------------------

}

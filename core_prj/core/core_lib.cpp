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
    qInfo("Initialising core layer");
    thread_ = new QThread;
    singleton_ = new Core;

    QObject::connect(thread_, QThread::finished, singleton_, Core::moveToMainThread);
}

//------------------------------------------------------------------------------

void start()
{
    qInfo("Moving core singleton to core thread");
    singleton_->moveToThread(thread_);

    qInfo("Starting core thread");
    thread_->start();
}

//------------------------------------------------------------------------------

void shutdown()
{
    qInfo("Shutting down core thread");
    thread_->exit();
    thread_->wait();
}

//------------------------------------------------------------------------------

Core *singleton()
{
    Q_ASSERT(singleton_);
    return singleton_;
}

//------------------------------------------------------------------------------

void finalise()
{
    qInfo("Finalising core layer");
    delete singleton_;
    delete thread_;

    singleton_ = nullptr;
    thread_ = nullptr;
}

//------------------------------------------------------------------------------

}

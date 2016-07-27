#include <QCoreApplication>
#include <QThread>

#include "Core.h"
#include "Crystal.h"

static QThread *thread_ = nullptr;
static Core *singleton_ = nullptr;

static int i_ = qRegisterMetaType<CrystalParameters>();

Core::Core()
    : QObject()
    , crystal_(new Crystal(this))
{
}

Core::~Core()
{
    delete crystal_;
}

void Core::init()
{
    qDebug("Initialising core layer");
    thread_ = new QThread;
    singleton_ = new Core;

    QObject::connect(thread_, QThread::finished, singleton_, Core::shutdown_);
}

void Core::start()
{
    qDebug("Moving core singleton to core thread");
    singleton_->moveToThread(thread_);

    qDebug("Starting core thread");
    thread_->start();
}

void Core::shutdown()
{
    qDebug("Shutting down core thread");
    thread_->exit();
    thread_->wait();
}

Core *Core::singleton()
{
    return singleton_;
}


void Core::finalise()
{
    qDebug("Finalising core layer");
    delete singleton_;
    delete thread_;

    singleton_ = nullptr;
    thread_ = nullptr;
}


void Core::setParameters(const CrystalParameters &params)
{
    crystal_->setParameters(params);
}

void Core::stopDevices()
{
    qDebug("Stopping devices");
}

void Core::shutdown_()
{
    qDebug("Moving core singleton to main thread");
    moveToThread(QCoreApplication::instance()->thread());
}

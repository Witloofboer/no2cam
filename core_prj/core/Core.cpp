#include "Core.h"

#include <QCoreApplication>

#include "Crystal.h"

//------------------------------------------------------------------------------

namespace core {

static int i_ = qRegisterMetaType<CrystalParameters>();

//------------------------------------------------------------------------------

Core::Core()
    : QObject()
    , crystal_(new Crystal(this))
{
}

//------------------------------------------------------------------------------

Core::~Core()
{
    delete crystal_;
}

//------------------------------------------------------------------------------

void Core::setParameters(const CrystalParameters &params)
{
    crystal_->setParameters(params);
}

//------------------------------------------------------------------------------

void Core::stopDevices()
{
    qDebug("Stopping devices");
}

//------------------------------------------------------------------------------

void Core::moveToMainThread()
{
    qDebug("Moving core singleton to main thread");
    moveToThread(QCoreApplication::instance()->thread());
}

//------------------------------------------------------------------------------

}

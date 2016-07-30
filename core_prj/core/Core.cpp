#include "Core.h"

#include <QCoreApplication>

#include "Crystal.h"

//------------------------------------------------------------------------------

namespace core {

//------------------------------------------------------------------------------

void Core::stopDevices()
{
    qInfo("Stopping devices");
}

//------------------------------------------------------------------------------

void Core::moveToMainThread()
{
    qInfo("Moving core singleton to main thread");
    moveToThread(QCoreApplication::instance()->thread());
}

//------------------------------------------------------------------------------

}

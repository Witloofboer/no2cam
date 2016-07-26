#include <QCoreApplication>

#include "CoreThread.h"
#include "Core.h"

void CoreThread::run()
{
    QThread::run();
    Core::singleton()->moveToThread(QCoreApplication::instance()->thread());
}

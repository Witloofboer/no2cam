#include <QApplication>

#include "core/mockups.h"
#include "gui/gui_global.h"

#include <QDebug>

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss.zzz} (%{threadid}) %{type}: %{message}");

    QApplication application(argc, argv);

    auto thermometer = new core::MockThermometer;
    auto camera = new core::MockCamera;
    auto driver = new core::MockAcousticDriver;

    init("mockup",
         QObject::tr("<p>Functional mockups are used for all devices.</p>"),
         thermometer, camera, driver);

    start();
    int result = application.exec();
    finalise();

    return result;
}

//------------------------------------------------------------------------------

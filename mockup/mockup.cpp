#include <QApplication>

#include "core/BaseCamera.h"
#include "core/BaseDriver.h"
#include "core/BaseGenerator.h"
#include "core/BaseTemperatureProbe.h"
#include "gui/gui_global.h"

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss.zzz} (%{threadid}) %{type}: %{message}");

    QApplication application(argc, argv);

    init("mockup",
         QObject::tr("<p>Functional mockups are used for all devices.</p>"),
         new core::MockTemperatureProbe,
         new core::MockCamera,
         new core::MockGenerator,
         new core::MockDriver);
    start();
    int result = application.exec();
    shutdown();
    finalise();

    return result;
}

//------------------------------------------------------------------------------

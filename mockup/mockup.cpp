#include <QApplication>

#include "core/mockups.h"
#include "gui/gui_global.h"

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss.zzz} (%{threadid}) %{type}: %{message}");

    QApplication application(argc, argv);

    auto probe = new core::MockProbe;
    auto camera = new core::MockCamera;
    auto generator = new core::MockGenerator;
    auto driver = new core::MockDriver;

    init("mockup",
         QObject::tr("<p>Functional mockups are used for all devices.</p>"),
         probe, camera, generator, driver);

    start();
    int result = application.exec();
    shutdown();
    finalise();

    return result;
}

//------------------------------------------------------------------------------

#include <QApplication>

#include "gui/gui_global.h"

#include "Camera.h"
#include "Driver.h"
#include "Generator.h"
#include "core/mockups.h"

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss.zzz} (%{threadid}) %{type}: %{message}");

    QApplication application(argc, argv);

    auto probe = new core::MockProbe;
    auto gen = new Generator;
    auto driver = new Driver(gen);
    auto camera = new HamamatsuCamera();

    camera->init();

    init("",
         QObject::tr("<p>Actual devices are used.</p>"),
         probe, camera, gen, driver);

    start();
    int result = application.exec();
    shutdown();

    camera->uninit();

    finalise();
    return result;
}

//------------------------------------------------------------------------------

#include <QApplication>

#include "gui/gui_global.h"

#include "Camera.h"
#include "Driver.h"
#include "Generator.h"
#include "core/mockups.h"
#include "probe.h"


//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss.zzz} (%{threadid}) %{type}: %{message}");

    QApplication application(argc, argv);

    //auto probe = new core::MockProbe;
    auto probe = new Probe;
    auto gen = new Generator;
    auto driver = new Driver(gen);
    //auto camera = new HamamatsuCamera();
    auto camera = new core::MockCamera();

    //bool ok = camera->init();
    probe->init();


    //if (!ok) return -1;

    init("",
         QObject::tr("<p>Actual devices are used.</p>"),
         probe, camera, gen, driver);

    start();
    int result = application.exec();

    //camera->uninit();
    probe-> uninit();
    finalise();
    return result;
}

//------------------------------------------------------------------------------

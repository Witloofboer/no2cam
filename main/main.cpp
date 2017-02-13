#include <QApplication>

#include "gui/gui_global.h"

#include "Camera.h"
#include "Driver.h"
#include "core/mockups.h"
#include "Probe.h"


//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss.zzz} (%{threadid}) %{type}: %{message}");

    QApplication application(argc, argv);

    //auto probe = new core::MockProbe;
    auto probe = new Probe;
    auto driver = new Driver();
    //auto camera = new HamamatsuCamera();
    auto camera = new core::MockCamera();

    //bool ok = camera->init();
    probe->init();

    //if (!ok) return -1;

    init("",
         QObject::tr("<p>Actual devices are used.</p>"),
         probe, camera, driver);

    start();
    int result = application.exec();

    //camera->uninit();
    probe-> uninit();
    finalise();
    return result;
}

//------------------------------------------------------------------------------

#include <QApplication>

#include "gui/gui_global.h"

#include "Camera.h"
#include "Driver.h"
#include "core/mockups.h"

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss.zzz} (%{threadid}) %{type}: %{message}");

    QApplication application(argc, argv);

    auto probe = new core::MockProbe;
    auto driver = new Driver();
    auto camera = new HamamatsuCamera();

    bool ok = camera->init();

    if (!ok) return -1;

    init("",
         QObject::tr("<p>Actual devices are used.</p>"),
         probe, camera, driver);

    start();
    int result = application.exec();

    camera->uninit();

    finalise();
    return result;
}

//------------------------------------------------------------------------------

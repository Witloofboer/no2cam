#include <QApplication>

#include "gui/gui_global.h"

#include "Camera.h"
#include "Driver.h"
#include "Generator.h"
#include "CrysTempProbe.h"

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss.zzz} (%{threadid}) %{type}: %{message}");

    Q_INIT_RESOURCE(scene);

    QApplication application(argc, argv);

    auto *gen = new Generator;
    auto *driver = new Driver(gen);
    auto *camera = new HamamatsuCamera();

    camera->init();

    init("0.3.0",
         new MockCrysTempProbe,
         camera,
         gen,
         driver);
    start();

    int result = application.exec();
    shutdown();

    camera->uninit();

    return result;
}

//------------------------------------------------------------------------------

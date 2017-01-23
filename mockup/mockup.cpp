#include <QApplication>

#include "gui/gui_global.h"

#include "MockCamera.h"
#include "MockDriver.h"
#include "MockGenerator.h"
#include "MockCrysTempProbe.h"

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss.zzz} (%{threadid}) %{type}: %{message}");

    Q_INIT_RESOURCE(scene);

    QApplication application(argc, argv);

    init("0.3.0 (mockup)",
         new MockCrysTempProbe,
         new MockCamera,
         new MockGenerator,
         new MockDriver);
    start();
    int result = application.exec();
    shutdown();
    finalise();

    return result;
}

//------------------------------------------------------------------------------

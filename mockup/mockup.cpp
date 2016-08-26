#include <QApplication>

#include "gui/gui_global.h"

#include "MockCamera.h"

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss} (%{threadid}) %{type}: %{message}");

    Q_INIT_RESOURCE(scene);

    QApplication application(argc, argv);

    init("0.2.0 (mockup)", new MockCamera);
    start();
    int result = application.exec();
    shutdown();
    finalise();

    return result;
}

//------------------------------------------------------------------------------

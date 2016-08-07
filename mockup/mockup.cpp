#include <QApplication>

#include "../gui/gui_global.h"

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss} (%{threadid}) %{type}: %{message}");

    Q_INIT_RESOURCE(camera);

    QApplication application(argc, argv);

    init("0.2.0 (mockup)");
    start();
    int result = application.exec();
    shutdown();
    finalise();

    return result;
}

//------------------------------------------------------------------------------

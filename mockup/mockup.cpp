#include <QApplication>

#include "../gui/gui_global.h"

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss} (%{threadid}) %{type}: %{message}");
    QApplication application(argc, argv);

    init("0.1.0 (mockup)");
    start();

    int result = application.exec();

    shutdown();
    finalise();

    return result;
}

//------------------------------------------------------------------------------

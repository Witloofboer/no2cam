#include <QApplication>

#include "../gui/gui_lib.h"
#include "../core/core_lib.h"

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss} (%{threadid}) %{type}: %{message}");
    QApplication application(argc, argv);

    core::init();
    gui::init("0.1.0 (mockup)");

    core::start();

    int result = application.exec();

    core::shutdown();

    gui::finalise();
    core::finalise();

    return result;
}

//------------------------------------------------------------------------------

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

    QString releaseNotes(
        "<p> This version contains a full implementation of the GUI (with the "
        "exception of the data folder selection) and the business logic "
        "required by the  GUI.</p>"
        "<p><b>Note that in this version all devices are mockups.</b></p>");

    init("0.3.0 (mockup)",
         releaseNotes,
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

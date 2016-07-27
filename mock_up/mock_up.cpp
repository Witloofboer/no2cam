#include <QApplication>


#include "../gui/MainWindow.h"
#include "../core/Core.h"

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time hh:mm:ss} (%{threadid}) %{type}: %{message}");
    QApplication application(argc, argv);

    QCoreApplication::setOrganizationName("BIRA-IASB");
    QCoreApplication::setApplicationName("NO2_Cam");

    Core::init();

    MainWindow w("0.1.0 (mockup)");
    w.show();

    Core::start();

    int result = application.exec();

    Core::shutdown();

    Core::finalise();

    return result;
}

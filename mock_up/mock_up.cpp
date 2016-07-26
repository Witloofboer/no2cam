#include <QApplication>

#include "../gui/MainWindow.h"
#include "../core/Core.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("BIRA-IASB");
    QCoreApplication::setApplicationName("NO2_Cam");

    MainWindow w("0.1.0 (mockup)");
    w.show();

    Core::singleton()->start();

    qDebug("Starting GUI thread");
    int result = a.exec();

    Core::singleton()->wait();
    return result;
}

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

    Core core;

    int result = a.exec();

    return result;
}

#include <QApplication>

#include "../gui/MainWindow.h"
#include "../core/Core.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w("0.1.0 (mockup)");
    w.show();

    gCore.start();

    int result = a.exec();

    gCore.quit();
    gCore.wait();

    return result;
}

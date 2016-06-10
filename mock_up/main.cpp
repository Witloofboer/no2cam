#include "../gui/MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(w.windowTitle() + " (Mock-Up)");
    w.show();

    return a.exec();
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "gui_global.h"

//------------------------------------------------------------------------------

class QAction;
class QCloseEvent;
class QStackedWidget;

namespace gui {

class ConfigurationDlg;
class SnapshotPane;

//------------------------------------------------------------------------------

class GUISHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &version, QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *event) override;

    private slots:
    void newSession();
    void loadSession();
    bool saveSession();
    bool saveAsSession();

    void switchMode();
    void configure();

    void cameraStatus();

    void releaseNotes();
    void about();

private:
    bool okToContinue();

    QStackedWidget *stackedWidget;
    QAction *snapshotModeAction;
    QAction *observationModeAction;
    QAction *sweepModeAction;

    ConfigurationDlg *configurationDlg;
    QString version;

    SnapshotPane *snapshotPane;
};

//------------------------------------------------------------------------------

}

#endif // MAINWINDOW_H

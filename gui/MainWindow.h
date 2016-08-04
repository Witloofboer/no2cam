#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "gui_global.h"

//------------------------------------------------------------------------------

class QAction;
class QCloseEvent;
class QShowEvent;
class QStackedWidget;

namespace gui {

class ConfigurationDlg;
class SnapshotPane;
class ObservationPane;
class SweepPane;

//------------------------------------------------------------------------------

class GUISHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &_version);

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

private slots:
    void displayConfigurationDlg();

private:
    bool okToContinue();

    QStackedWidget *_stackedWdgt;
    QAction *_snapshotModeActn;
    QAction *_observationModeActn;
    QAction *_sweepModeActn;

    ConfigurationDlg *_configurationDlg;
    QString _version;

    SnapshotPane *_snapshotPane;
    ObservationPane *_observationPane;
    SweepPane *_sweepPane;
};

//------------------------------------------------------------------------------

}

#endif // MAINWINDOW_H

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

namespace core {
class Core;
class Crystal;
}

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
    MainWindow(core::Crystal *crystal,
               core::Core *coreObj,
               const QString &_version);

signals:
    void start(bool burst, bool record);
    void stop();

public slots:
    void updateState(bool isAppReady);

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
    QAction *_configParamActn;

    ConfigurationDlg *_configDlg;
    QString _version;

    SnapshotPane *_snapshotPane;
    ObservationPane *_observationPane;
    SweepPane *_sweepPane;
};

//------------------------------------------------------------------------------

}

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "gui_global.h"

//------------------------------------------------------------------------------

class QAction;
class QCloseEvent;
class QLabel;
class QShowEvent;
class QStackedWidget;
class QTimer;

namespace core {
class Manager;
class Crystal;
class BaseTemperatureProbe;
}

namespace gui {

class BaseParameterPane;
class CameraBtnBox;
class ConfigurationDlg;
class HistogramWidget;
class LineEdit;
class ObservationParameterPane;
class SnapshotWidget;
class SnapshotParameterPane;
class SweepParameterPane;

//------------------------------------------------------------------------------

class GUISHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(core::Crystal *crystal,
               core::Manager *coreObj,
               const QString &version,
               const QString &devicesNotes);

signals:
    void onStop();
    void shutdownRequested();
    void temperaturePeriodUpdated(int temperaturePeriod);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onStart(bool burst, bool record);
    void onSwitchMode();
    void refreshBtns();
    void onSelectFolder();
    void onConfigure();

    void onDisplayReleaseNotes();
    void onAbout();

    void onUpdateTemperature(double temperature);
    void onUpdateApplicationReadiness(bool isAppReady);
    void onDisplaySnapshot();
    void onFileCreationError(QString datafolder, QString filename);
    void onFileWritingError(QString datafolder, QString filename);
    void onDisplayInformation(QString msg);

    void onDisplayConfigurationDlg();
    void onParametersUpdated();
    void onInfoTimer();

private:
    BaseParameterPane *currentPane();
    void persiste();
    void restore();
    void refreshWindowTitle();

    core::Manager *_coreManager;

    ConfigurationDlg *_configDlg;
    QStackedWidget *_stackedWdgt;
    QAction *_selectFolderActn;
    QAction *_snapshotModeActn;
    QAction *_observationModeActn;
    QAction *_sweepModeActn;
    QAction *_configParamActn;

    QString _dataFolder;
    QString _version;
    QString _devicesNotes;

    SnapshotParameterPane    *_snapshotPane;
    ObservationParameterPane *_observationPane;
    SweepParameterPane       *_sweepPane;
    CameraBtnBox *_cameraBtnBox;

    LineEdit        *_sessionEdit;
    SnapshotWidget  *_snapshot;
    HistogramWidget *_histogram;
    QLabel          *_infoWdgt;
    QLabel          *_temperatureWdgt;
    QTimer          *_infoT;
};

//------------------------------------------------------------------------------

}

#endif // MAINWINDOW_H

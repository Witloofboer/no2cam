#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "gui_global.h"

#include "core/BaseCamera.h"

//------------------------------------------------------------------------------

class QAction;
class QCloseEvent;
class QShowEvent;
class QStackedWidget;

namespace core {
class Core;
class Crystal;
class AbstractCrysTempProbe;
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
               core::AbstractCrysTempProbe *crysTempProb,
               core::Core *coreObj,
               const QString &version,
               const QString &releaseNotes);

signals:
    void stopped();

public slots:
    void updateGuiState(bool isAppReady);
    void updateSnapshot();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void start(bool burst, bool record);
    void switchMode();
    void refreshBtns();
    void selectFolder();
    void configure();

    void displayReleaseNotes();
    void about();

    void displayConfigurationDlg();
    void displayErrorOnFileCreation(QString datafolder, QString filename);
    void displayErrorOnFileWritting(QString datafolder, QString filename);

private:
    BaseParameterPane *currentPane();
    void persiste();
    void restore();
    void refreshWindowTitle();

    core::Core *_coreInstance;

    ConfigurationDlg *_configDlg;
    QStackedWidget *_stackedWdgt;
    QAction *_selectFolderActn;
    QAction *_snapshotModeActn;
    QAction *_observationModeActn;
    QAction *_sweepModeActn;
    QAction *_configParamActn;

    QString _dataFolder;
    QString _version;
    QString _releaseNotes;

    SnapshotParameterPane    *_snapshotPane;
    ObservationParameterPane *_observationPane;
    SweepParameterPane       *_sweepPane;
    CameraBtnBox *_cameraBtnBox;

    LineEdit        *_sessionEdit;
    SnapshotWidget  *_snapshot;
    HistogramWidget *_histogram;
};

//------------------------------------------------------------------------------

}

#endif // MAINWINDOW_H

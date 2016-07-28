#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "gui_global.h"

class QAction;
class QClauseEvent;
class QStackedWidget;
class ConfigurationDlg;

class GUISHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &version, QWidget *parent = 0);
    ~MainWindow();

    static MainWindow *instance() {return instance_;}

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
    void notImplemented(const QString& feature);

    QStackedWidget *stackedWidget_;
    QAction *snapshotModeAction_;
    QAction *observationModeAction_;
    QAction *sweepModeAction_;

    ConfigurationDlg *configurationDlg_;
    QString version_;

    static MainWindow *instance_;
};

#endif // MAINWINDOW_H

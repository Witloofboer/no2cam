#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "gui_global.h"

class QStackedWidget;
class QAction;
class ConfigurationDlg;

class GUISHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &version, QWidget *parent = 0);
    ~MainWindow();

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
};

#endif // MAINWINDOW_H

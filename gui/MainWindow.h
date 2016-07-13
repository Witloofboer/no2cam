#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
class QStackedWidget;
class QAction;
class ConfigurationDlg;

class MainWindow : public QMainWindow
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

    QStackedWidget *stackedWidget;
    QAction *snapshotModeAction;
    QAction *observationModeAction;
    QAction *sweepModeAction;

    ConfigurationDlg *configurationDlg;
    QString version;
};

#endif // MAINWINDOW_H

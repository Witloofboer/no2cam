#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QStackedWidget;
class QAction;
class ConfigurationDlg;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void newSession();
    void loadSession();
    bool saveSession();
    bool saveAsSession();

    void switchMode();
    void configure();
    void releaseNotes();
    void about();


private:
    bool okToContinue();
    void notImplemented(const QString& feature);

    QStackedWidget *mStackedWidget;
    QAction *mSnapshotModeAction;
    QAction *mObservationModeAction;
    QAction *mSweepModeAction;

    ConfigurationDlg *mConfigurationDlg;
};

#endif // MAINWINDOW_H

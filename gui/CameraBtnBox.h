#ifndef CAMERABTNBOX_H
#define CAMERABTNBOX_H

#include <QGroupBox>

//------------------------------------------------------------------------------

class QPushButton;
class QWidget;

namespace gui {

class MainWindow;

//------------------------------------------------------------------------------

class CameraBtnBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit CameraBtnBox(MainWindow *mainWindow);
    void updateState(bool isAppReady);

signals:
    void start(bool burst, bool record);

public slots:
    void enableBtns(bool playEnabled, bool recordEnabled);

private:
    void refreshBtns();

private slots:
    void singleClicked();
    void burstClicked();

private:
    MainWindow *_mainWindow;
    QPushButton *_singleBtn;
    QPushButton *_burstBtn;
    QPushButton *_recordBtn;
    QPushButton *_stopBtn;

    bool _isAppReady;
    bool _playEnabled;
    bool _recordEnabled;
};

//------------------------------------------------------------------------------

}

#endif // CAMERABTNBOX_H

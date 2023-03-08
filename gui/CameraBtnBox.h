#ifndef CAMERABTNBOX_H
#define CAMERABTNBOX_H

#include <QGroupBox>

//------------------------------------------------------------------------------

class QPushButton;

namespace gui {

//------------------------------------------------------------------------------

class CameraBtnBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit CameraBtnBox();
    void updateState(bool isAppReady);

signals:
    void started(bool burst, bool record);
    void stopped();

public slots:
    void enableBtns(bool playEnabled, bool recordEnabled);

private:
    void refreshBtns();

private slots:
    void start();

private:
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

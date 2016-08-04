#ifndef CAMERABTNBOX_H
#define CAMERABTNBOX_H

#include <QGroupBox>

//------------------------------------------------------------------------------

class QPushButton;
class QWidget;

namespace gui {

//------------------------------------------------------------------------------

class CameraBtnBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit CameraBtnBox();

signals:
    void start(bool burst, bool record);
    void stop();

public slots:
    void coreReady();
    void enableBtns(bool playEnabled, bool recordEnabled);

private:
    void refreshBtns();

private slots:
    void singleClicked();
    void burstClicked();

private:
    QPushButton *_singleBtn;
    QPushButton *_burstBtn;
    QPushButton *_recordBtn;
    QPushButton *_stopBtn;

    bool _coreReady;
    bool _playEnabled;
    bool _recordEnabled;
};

//------------------------------------------------------------------------------

}

#endif // CAMERABTNBOX_H

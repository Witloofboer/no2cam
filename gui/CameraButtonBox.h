#ifndef CAMERABUTTONPANEL_H
#define CAMERABUTTONPANEL_H

#include <QGroupBox>

//------------------------------------------------------------------------------

class QPushButton;
class QWidget;

namespace gui {

//------------------------------------------------------------------------------

class CameraButtonBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit CameraButtonBox(QWidget *parent = 0);

    enum Event {kTakeOne, kBurst, kStop};

signals:
    void play(bool burst, bool record);
    void stop();

public slots:
    void done();
    void setButtons(bool playEnabled, bool recordEnabled);

private:
    void refreshBtnStatus();

private slots:
    void singleClicked();
    void burstClicked();

private:
    QPushButton *singleBtn;
    QPushButton *burstBtn;
    QPushButton *recordBtn;
    QPushButton *stopBtn;

    bool applicationReady;
    bool playEnabled;
    bool recordEnabled;
};

//------------------------------------------------------------------------------

}

#endif // CAMERABUTTONPANEL_H

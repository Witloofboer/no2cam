#ifndef CAMERABUTTONPANEL_H
#define CAMERABUTTONPANEL_H

#include <QGroupBox>

class QPushButton;
class QWidget;

class CameraButtonBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit CameraButtonBox(QWidget *parent = 0);

    enum Event {kTakeOne, kBurst, kRecord, kEndRecord, kStop};

signals:
    void clicked(Event event);

private slots:
    void takeOneClicked();
    void burstClicked();
    void recordClicked();
    void stopClicked();

private:
    QPushButton *mTakeOneBtn;
    QPushButton *mBurstBtn;
    QPushButton *mRecordBtn;
    QPushButton *mStopBtn;
};

#endif // CAMERABUTTONPANEL_H

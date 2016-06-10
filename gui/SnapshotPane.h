#ifndef SNAPSHOTWIDGET_H
#define SNAPSHOTWIDGET_H

#include "AbstractMainPane.h"

class QLineEdit;
class QRadioButton;

class SnapshotPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit SnapshotPane(QWidget *parent = 0);

signals:

private slots:
    void parameterModeSwitch();

private:
    QRadioButton *mOpticBtn;
    QLineEdit *mWavelength;
    QLineEdit *mAccousticFreq;
    QLineEdit *mAccousticPower;
    QLineEdit *mEET;
    QLineEdit *mCoolDown;
    QLineEdit *mSessionName;
};

#endif // SNAPSHOTWIDGET_H

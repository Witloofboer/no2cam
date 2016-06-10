#ifndef SWEEPWIDGET_H
#define SWEEPWIDGET_H

#include "AbstractMainPane.h"

class QLineEdit;

class SweepingPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit SweepingPane(QWidget *parent = 0);

signals:

public slots:

private:
    QLineEdit *mWavelength1;
    QLineEdit *mWavelength2;
    QLineEdit *mWavelengthDelta;
    QLineEdit *mEET;
    QLineEdit *mCoolDown;
    QLineEdit *mSessionName;
};

#endif // SWEEPWIDGET_H

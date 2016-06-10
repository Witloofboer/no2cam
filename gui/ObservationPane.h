#ifndef OBSERVATIONWIDGET_H
#define OBSERVATIONWIDGET_H

#include "AbstractMainPane.h"

class QLineEdit;

class ObservationPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit ObservationPane(QWidget *parent = 0);

signals:

private:
    QLineEdit *mWavelength1;
    QLineEdit *mWavelength2;
    QLineEdit *mEET;
    QLineEdit *mSnapshotsPerObs;
    QLineEdit *mCoolDown;
    QLineEdit *mSessionName;
};

#endif // OBSERVATIONWIDGET_H

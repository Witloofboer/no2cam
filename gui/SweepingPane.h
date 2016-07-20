#ifndef SWEEPWIDGET_H
#define SWEEPWIDGET_H

#include "AbstractMainPane.h"

class QLineEdit;
class DoubleLineEdit;

class SweepingPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit SweepingPane(QWidget *parent = 0);

signals:

public slots:

private:
    DoubleLineEdit *wl1_;
    DoubleLineEdit *wl2_;
    DoubleLineEdit *wlDelta_;
    DoubleLineEdit *eet_;
    DoubleLineEdit *cooldown_;
    QLineEdit  *session_;
};

#endif // SWEEPWIDGET_H

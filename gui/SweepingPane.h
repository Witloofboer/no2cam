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
    QLineEdit *wl1_;
    QLineEdit *wl2_;
    QLineEdit *wlDelta_;
    QLineEdit *eet_;
    QLineEdit *cooldown_;
    QLineEdit *session_;
};

#endif // SWEEPWIDGET_H

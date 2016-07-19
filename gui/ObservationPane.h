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
    QLineEdit *wl1_;
    QLineEdit *wl2_;
    QLineEdit *eet_;
    QLineEdit *cooldown_;
    QLineEdit *shots_;
    QLineEdit *session_;
};

#endif // OBSERVATIONWIDGET_H

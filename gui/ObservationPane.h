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
    QLineEdit *wl1;
    QLineEdit *wl2;
    QLineEdit *eet;
    QLineEdit *cooldown;
    QLineEdit *shots;
    QLineEdit *session;
};

#endif // OBSERVATIONWIDGET_H

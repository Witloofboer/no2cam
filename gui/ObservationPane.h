#ifndef OBSERVATIONWIDGET_H
#define OBSERVATIONWIDGET_H

#include "AbstractMainPane.h"

class QLineEdit;
class DoubleLineEdit;

class ObservationPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit ObservationPane(QWidget *parent = 0);

signals:

private:
    DoubleLineEdit *wl1_;
    DoubleLineEdit *wl2_;
    DoubleLineEdit *eet_;
    DoubleLineEdit *cooldown_;
    DoubleLineEdit *shots_;
    QLineEdit  *session_;
};

#endif // OBSERVATIONWIDGET_H

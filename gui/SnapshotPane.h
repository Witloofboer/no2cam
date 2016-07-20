#ifndef SNAPSHOTWIDGET_H
#define SNAPSHOTWIDGET_H

#include "AbstractMainPane.h"

class QLineEdit;
class QRadioButton;

class DoubleLineEdit;

class SnapshotPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit SnapshotPane(QWidget *parent = 0);

signals:

private slots:
    void parameterModeSwitch();

private:
    QRadioButton *opticBtn_;
    DoubleLineEdit *wl_;
    DoubleLineEdit *aF_;
    DoubleLineEdit *aP_;
    DoubleLineEdit *eet_;
    DoubleLineEdit *cooldown_;
    QLineEdit  *session_;
};

#endif // SNAPSHOTWIDGET_H

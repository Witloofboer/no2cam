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
    QRadioButton *opticBtn;
    QLineEdit *wl;
    QLineEdit *aF;
    QLineEdit *aP;
    QLineEdit *eet;
    QLineEdit *cooldown;
    QLineEdit *session;
};

#endif // SNAPSHOTWIDGET_H

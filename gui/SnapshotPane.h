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
    QRadioButton *opticBtn_;
    QLineEdit *wl_;
    QLineEdit *aF_;
    QLineEdit *aP_;
    QLineEdit *eet_;
    QLineEdit *cooldown_;
    QLineEdit *session_;
};

#endif // SNAPSHOTWIDGET_H

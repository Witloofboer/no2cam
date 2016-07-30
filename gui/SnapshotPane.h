#ifndef SNAPSHOTWIDGET_H
#define SNAPSHOTWIDGET_H

#include "AbstractMainPane.h"

//------------------------------------------------------------------------------

class QRadioButton;

namespace gui {

class ConfigurationDlg;
class DoubleLineEdit;
class IntLineEdit;
class LineEdit;

//------------------------------------------------------------------------------

class SnapshotPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit SnapshotPane(ConfigurationDlg * configurationDlg);
    void persiste() const;

signals:

public slots:
    void refreshParameters();

private slots:
    void switchParamMode();

private:

    void restore();
    ConfigurationDlg *configurationDlg;
    QRadioButton   *wavelengthBtn;
    QRadioButton   *acousticBtn;
    DoubleLineEdit *wavelengthEdit;
    DoubleLineEdit *frequencyEdit;
    IntLineEdit    *powerEdit;
    IntLineEdit    *exposureEdit;
    IntLineEdit    *cooldownEdit;
    LineEdit       *sessionEdit;
};

//------------------------------------------------------------------------------

}

#endif // SNAPSHOTWIDGET_H

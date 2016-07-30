#ifndef CONFIGURATIONDLG_H
#define CONFIGURATIONDLG_H

#include <QDialog>

#include "Crystal.h"

//------------------------------------------------------------------------------

class QDialogButtonBox;

namespace gui {

class DoubleLineEdit;

//------------------------------------------------------------------------------

class ConfigurationDlg : public QDialog
{
    Q_OBJECT

public:
    ConfigurationDlg(QWidget *parent=0);

public slots:
    void display();

private slots:
    void updateDlgBtns();

private:
    void pushParamsToGui();

    DoubleLineEdit *cutAngle_;       // Cut angle
    DoubleLineEdit *incidentAngle_;  // Incident angle
    DoubleLineEdit *transHeight_;    // Transducer height
    DoubleLineEdit *transLength_;    // Transducer length
    QDialogButtonBox *buttonBox_;

    core::Crystal crystal;

};

//------------------------------------------------------------------------------

}

#endif // CONFIGURATIONDLG_H

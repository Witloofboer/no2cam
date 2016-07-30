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

    DoubleLineEdit *cutAngle;       // Cut angle
    DoubleLineEdit *incidentAngle;  // Incident angle
    DoubleLineEdit *transHeight;    // Transducer height
    DoubleLineEdit *transLength;    // Transducer length
    QDialogButtonBox *buttonBox;

    core::Crystal crystal;

};

//------------------------------------------------------------------------------

}

#endif // CONFIGURATIONDLG_H

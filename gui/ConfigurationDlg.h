#ifndef CONFIGURATIONDLG_H
#define CONFIGURATIONDLG_H

#include <QDialog>

#include "Crystal.h"

//------------------------------------------------------------------------------

class QDialogButtonBox;

namespace gui {

class MainWindow;
class DoubleLineEdit;

//------------------------------------------------------------------------------

class ConfigurationDlg : public QDialog
{
    Q_OBJECT

public:
    ConfigurationDlg(MainWindow *mainWindow);
    const core::Crystal &refCrystal() const {return crystal;}
    void persiste() const;
    bool isValid() const;

signals:
    void crystalUpdated();

public slots:
    void display(bool abortEnabled=false);

private slots:
    void updateOkBtn();
    void pushToGui();
    void pushToCrystal();

private:
    void restore();

    MainWindow *mainWindow;
    DoubleLineEdit *cutAngleEdit;    // Cut angle
    DoubleLineEdit *incidentEdit;    // Incident angle
    DoubleLineEdit *transHeightEdit; // Transducer height
    DoubleLineEdit *transLengthEdit; // Transducer length
    QDialogButtonBox *buttonBox;

    core::Crystal crystal;
};

//------------------------------------------------------------------------------

}

#endif // CONFIGURATIONDLG_H

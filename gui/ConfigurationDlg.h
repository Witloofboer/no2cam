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
    ConfigurationDlg(MainWindow *mainWindow, core::Crystal *crystal);
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

    MainWindow *_mainWindow;
    DoubleLineEdit *_cutAngleEdit;    // Cut angle
    DoubleLineEdit *_incidentEdit;    // Incident angle
    DoubleLineEdit *_heightEdit; // Transducer height
    DoubleLineEdit *_lengthEdit; // Transducer length
    QDialogButtonBox *_buttonBox;

    core::Crystal *_crystal;
};

//------------------------------------------------------------------------------

}

#endif // CONFIGURATIONDLG_H

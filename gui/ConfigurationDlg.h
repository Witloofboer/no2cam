#ifndef CONFIGURATIONDLG_H
#define CONFIGURATIONDLG_H

#include <QDialog>

//------------------------------------------------------------------------------

class QDialogButtonBox;

namespace core {
class Crystal;
}

namespace gui {

class MainWindow;
class DoubleLineEdit;
class IntLineEdit;

//------------------------------------------------------------------------------

class ConfigurationDlg : public QDialog
{
    Q_OBJECT

public:
    ConfigurationDlg(MainWindow *mainWindow,
                     core::Crystal *crystal);
    void persiste() const;
    bool isValid() const;

signals:
    void crystalUpdated();

public slots:
    void display(bool abortEnabled=false);

    const int &stabilisationTime() const;

private slots:
    void updateOkBtn();
    void ackGuiValues();
    void restoreGuiValues();

private:
    void restore();

    MainWindow *_mainWindow;
    DoubleLineEdit *_cutAngleEdit; // Cut angle
    DoubleLineEdit *_incidentEdit; // Incident angle
    DoubleLineEdit *_heightEdit;   // Transducer height
    DoubleLineEdit *_lengthEdit;   // Transducer length

    IntLineEdit *_stabilTimeEdit;
    int _stabilisationTime;

    QDialogButtonBox *_buttonBox;

    core::Crystal *_crystal;
};

//------------------------------------------------------------------------------

}

#endif // CONFIGURATIONDLG_H

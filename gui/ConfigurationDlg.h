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
class PosIntLineEdit;

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
    void parametersUpdated();

public slots:
    void onDisplay(bool abortEnabled=false);

    int stabilisingTime() const;
    int temperaturePeriod() const;

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
    DoubleLineEdit *_lengthEdit;   // Transducer lenght

    IntLineEdit *_stabilisationTimeEdit;
    int _stabilisingTime;

    PosIntLineEdit *_temperaturePeriodEdit;
    int _temperaturePeriod;

    QDialogButtonBox *_buttonBox;

    core::Crystal *_crystal;
};

//------------------------------------------------------------------------------

}

#endif // CONFIGURATIONDLG_H

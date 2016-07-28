#ifndef CONFIGURATIONDLG_H
#define CONFIGURATIONDLG_H

#include <QDialog>

//------------------------------------------------------------------------------

class QDialogButtonBox;

namespace core {
    class CrystalParameters;
}

namespace gui {

class DoubleLineEdit;

//------------------------------------------------------------------------------

class ConfigurationDlg : public QDialog
{
    Q_OBJECT

public:
    ConfigurationDlg(QWidget *parent=0);
    ~ConfigurationDlg();

public slots:
    void display();

private slots:
    void updateDlgBtns();

signals:
    void parametersUpdated(const core::CrystalParameters& params);

private:
    DoubleLineEdit *cutAngle_;       // Cut angle
    DoubleLineEdit *incidentAngle_;  // Incident angle
    DoubleLineEdit *transHeight_;    // Transducer height
    DoubleLineEdit *transLength_;    // Transducer length

    core::CrystalParameters *params_;
    QDialogButtonBox *buttonBox_;
};

//------------------------------------------------------------------------------

}

#endif // CONFIGURATIONDLG_H

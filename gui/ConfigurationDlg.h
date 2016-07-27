#ifndef CONFIGURATIONDLG_H
#define CONFIGURATIONDLG_H

#include <QDialog>

class CrystalParameters;
class DoubleLineEdit;

class ConfigurationDlg : public QDialog
{
    Q_OBJECT

public:
    ConfigurationDlg(QWidget *parent=0);
    ~ConfigurationDlg();

public slots:
    void display();

signals:
    void parametersUpdated(const CrystalParameters& params);

private:
    DoubleLineEdit* cutAngle_;       // Cut angle
    DoubleLineEdit* incidentAngle_;  // Incident angle
    DoubleLineEdit* transHeight_;    // Transducer height
    DoubleLineEdit* transLength_;    // Transducer length

    CrystalParameters* params_;
};

#endif // CONFIGURATIONDLG_H

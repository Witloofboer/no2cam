#ifndef CONFIGURATIONDLG_H
#define CONFIGURATIONDLG_H

#include <QDialog>

class QLineEdit;

class ConfigurationDlg : public QDialog
{
public:
    ConfigurationDlg(QWidget *parent=0);

private:
    QLineEdit* cutAngle_;       // Cut angle
    QLineEdit* IncidentAngle_;  // Incident angle
    QLineEdit* transHeight_;    // Transducer height
    QLineEdit* transLength_;    // Transducer length
    QLineEdit* elasticCoeff_;   // Photo-elastic coeff.
};

#endif // CONFIGURATIONDLG_H

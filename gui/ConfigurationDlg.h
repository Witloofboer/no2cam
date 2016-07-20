#ifndef CONFIGURATIONDLG_H
#define CONFIGURATIONDLG_H

#include <QDialog>

#include <core/Crystal.h>

class DoubleLineEdit;

class ConfigurationDlg : public QDialog
{
    Q_OBJECT

public:
    ConfigurationDlg(QWidget *parent=0);

public slots:
    void display();

signals:
    void parametersUpdated(CrystalParameters crystalP);

private:
    DoubleLineEdit* cutAngle_;       // Cut angle
    DoubleLineEdit* incidentAngle_;  // Incident angle
    DoubleLineEdit* transHeight_;    // Transducer height
    DoubleLineEdit* transLength_;    // Transducer length

    CrystalParameters parameters_;
};

#endif // CONFIGURATIONDLG_H

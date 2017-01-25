#ifndef BASEPARAMETERPANE_H
#define BASEPARAMETERPANE_H

#include <QWidget>
#include <QString>

//------------------------------------------------------------------------------

class QGridLayout;
class QGroupBox;
class QVBoxLayout;
class QSettings;

namespace gui {

class IntLineEdit;

//------------------------------------------------------------------------------

class BaseParameterPane : public QWidget
{
    Q_OBJECT
public:
    explicit BaseParameterPane();

    virtual void start(bool burst,
                       bool record,
                       double stabilisationTime,
                       const QString& session,
                       const QString& dataFolder) =0;

    virtual bool areParametersValid() const;

signals:
    parametersChanged();


protected:
    void updateState(bool isAppReady);
    void putInGrid(int &row);

    void persiste(QSettings &settings) const;
    void restore(QSettings &settings);

    QGroupBox    *_parameterBox;
    QGridLayout  *_paramBoxLayout;

    IntLineEdit  *_exposureEdit;
    IntLineEdit  *_cooldownTimeEdit;
    IntLineEdit  *_cooldownPwrEdit;
};

//------------------------------------------------------------------------------

}

#endif // BASEPARAMETERPANE_H

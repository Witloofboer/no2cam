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

class MainWindow;
class IntLineEdit;

//------------------------------------------------------------------------------

class BaseParameterPane : public QWidget
{
    Q_OBJECT
public:
    explicit BaseParameterPane(MainWindow *mainWindow);

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

    MainWindow   *_mainWindow;
    QGroupBox    *_parameterBox;
    QGridLayout  *_paramBoxLayout;

    IntLineEdit  *_exposureEdit;
    IntLineEdit  *_cooldownTimeEdit;
};

//------------------------------------------------------------------------------

}

#endif // BASEPARAMETERPANE_H

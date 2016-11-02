#ifndef BASEPARAMETERPANE_H
#define BASEPARAMETERPANE_H

#include <QWidget>

//------------------------------------------------------------------------------

class QGridLayout;
class QGroupBox;
class QLabel;
class QVBoxLayout;
class QSettings;

namespace gui {

class CameraBtnBox;
class IntLineEdit;
class LineEdit;
class MainWindow;

//------------------------------------------------------------------------------

class BaseParameterPane : public QWidget
{
    Q_OBJECT
public:
    explicit BaseParameterPane(MainWindow* mainWindow);
    void updateState(bool isAppReady);

protected slots:
    void refreshBtns();
    virtual void start(bool burst, bool record) =0;

protected:
    void putInGrid(int &row);
    virtual bool areParametersValid() const;

    void persiste(QSettings &settings) const;
    void restore(QSettings &settings);

    QGroupBox    *_parameterBox;
    QGridLayout  *_paramBoxLayout;
    CameraBtnBox *_cameraBtnBox;

    IntLineEdit  *_exposureEdit;
    IntLineEdit  *_cooldownTimeEdit;
    IntLineEdit  *_cooldownPwrEdit;

    LineEdit     *_sessionEdit;
};

//------------------------------------------------------------------------------

}

#endif // BASEPARAMETERPANE_H

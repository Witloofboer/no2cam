#ifndef ABSTRACTMAINPANE_H
#define ABSTRACTMAINPANE_H

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

class AbstractMainPane : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractMainPane(MainWindow* mainWindow);
    void updateState(bool isAppReady);

protected slots:
    void refreshBtns();
    virtual void start(bool burst, bool record) =0;

protected:
    void putInGrid(int &row);
    virtual bool areParametersValid() const;

    void persiste(QSettings &settings) const;
    void restore(QSettings &settings);

    QVBoxLayout  *_leftLayout;
    QGridLayout  *_paramBoxLayout;
    QGroupBox    *_snapshotBox;
    CameraBtnBox *_cameraBtnBox;

    IntLineEdit  *_exposureEdit;
    IntLineEdit  *_cooldownTimeEdit;
    IntLineEdit  *_cooldownPwrEdit;

    LineEdit     *_sessionEdit;
};

//------------------------------------------------------------------------------

}

#endif // ABSTRACTMAINPANE_H

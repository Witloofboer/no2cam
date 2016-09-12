#ifndef ABSTRACTMAINPANE_H
#define ABSTRACTMAINPANE_H

#include <QWidget>

#include <core/AbstractCamera.h>

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
    void updateSnapshot(core::Snapshot& snapshot);

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
    QLabel       *_snapshotLbl;

    IntLineEdit  *_exposureEdit;
    IntLineEdit  *_cooldownTimeEdit;
    IntLineEdit  *_cooldownPwrEdit;

    LineEdit     *_sessionEdit;

    QImage       _snapshotImg;
};

//------------------------------------------------------------------------------

}

#endif // ABSTRACTMAINPANE_H

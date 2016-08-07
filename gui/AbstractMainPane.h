#ifndef ABSTRACTMAINPANE_H
#define ABSTRACTMAINPANE_H

#include <QWidget>

//------------------------------------------------------------------------------

class QGridLayout;
class QGroupBox;
class QLabel;
class QVBoxLayout;

namespace gui {

class CameraBtnBox;
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

    virtual bool areParametersValid() const =0;

    QVBoxLayout  *_leftLayout;
    QGridLayout  *_paramBoxLayout;
    QGroupBox    *_snapshotBox;
    CameraBtnBox *_cameraBtnBox;
    LineEdit     *_sessionEdit;
};

//------------------------------------------------------------------------------

}

#endif // ABSTRACTMAINPANE_H

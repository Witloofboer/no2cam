#ifndef ABSTRACTMAINPANE_H
#define ABSTRACTMAINPANE_H

#include <QWidget>

//------------------------------------------------------------------------------

class QGridLayout;
class QGroupBox;
class QLabel;
class QVBoxLayout;

namespace core {

class Crystal;

}

namespace gui {

class CameraBtnBox;

//------------------------------------------------------------------------------

class AbstractMainPane : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractMainPane(const core::Crystal& crystal);

signals:

public slots:

protected:
    QVBoxLayout *_leftLayout;
    QGridLayout *_paramBoxLayout;
    QGroupBox   *_snapshotBox;
    CameraBtnBox *_cameraBtnBox;

    const core::Crystal &_crystal;
};

//------------------------------------------------------------------------------

}

#endif // ABSTRACTMAINPANE_H

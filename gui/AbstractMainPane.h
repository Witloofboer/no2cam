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

//------------------------------------------------------------------------------

class AbstractMainPane : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractMainPane(const core::Crystal& crystal);

signals:

public slots:

protected:
    QVBoxLayout *leftLayout;
    QGridLayout *paramBoxLayout;
    QGroupBox   *snapshotBox;
    const core::Crystal &crystal;
};

//------------------------------------------------------------------------------

}

#endif // ABSTRACTMAINPANE_H

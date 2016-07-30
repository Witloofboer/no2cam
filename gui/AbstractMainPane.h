#ifndef ABSTRACTMAINPANE_H
#define ABSTRACTMAINPANE_H

#include <QWidget>

//------------------------------------------------------------------------------

class QGridLayout;
class QGroupBox;
class QLabel;
class QVBoxLayout;

namespace gui {

//------------------------------------------------------------------------------

class AbstractMainPane : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractMainPane(QWidget *parent = 0);

signals:

public slots:

protected:
    QVBoxLayout *leftLayout;
    QGridLayout *paramBoxLayout;
    QGroupBox   *snapshotBox;
};

//------------------------------------------------------------------------------

}

#endif // ABSTRACTMAINPANE_H

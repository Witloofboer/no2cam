#ifndef ABSTRACTMAINPANE_H
#define ABSTRACTMAINPANE_H

#include <QWidget>

class QGridLayout;
class QGroupBox;
class QLabel;
class QVBoxLayout;

class AbstractMainPane : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractMainPane(QWidget *parent = 0);

signals:

public slots:

protected:
    QVBoxLayout *leftLayout_;
    QGridLayout *paramBox_;
    QGroupBox   *snapshotBox_;
};

#endif // ABSTRACTMAINPANE_H

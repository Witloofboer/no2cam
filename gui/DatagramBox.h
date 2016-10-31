#ifndef INTENSITYDATAGRAM_H
#define INTENSITYDATAGRAM_H

#include <QGroupBox>
#include <QPainter>

class QPixmap;
class QLabel;

namespace gui {

//------------------------------------------------------------------------------

typedef quint16 Datagram[256];

class DatagramBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit DatagramBox(QWidget *parent = 0);

    void display(Datagram datagram);
signals:

public slots:

private:
    QPixmap *_pixmap;
    QPainter _pixPaint;
    QLabel  *_datagramLbl;

};

//------------------------------------------------------------------------------

}
#endif // INTENSITYDATAGRAM_H

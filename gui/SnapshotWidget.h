#ifndef SNAPSHOTWIDGET_H
#define SNAPSHOTWIDGET_H

#include <QLabel>
#include <QPainter>

#include "HistogramWidget.h"

class QPixmap;

namespace gui {

//------------------------------------------------------------------------------

class SnapshotWidget : public QLabel
{
    Q_OBJECT
public:
    explicit SnapshotWidget(QWidget *parent = 0);

    void update();
    const HistogramWidget::Data& histogramData() const {
        return _histogramData;
    }

signals:

public slots:

private:
    QImage   _image;
    QPainter _pixPaint;
    HistogramWidget::Data _histogramData;
};

//------------------------------------------------------------------------------

}
#endif // SNAPSHOTWIDGET_H

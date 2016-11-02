#include "SnapshotWidget.h"

#include "../core/ImageBuffer.h"

namespace gui {

//------------------------------------------------------------------------------

SnapshotWidget::SnapshotWidget(QWidget *parent)
    : QLabel(parent)
    , _image(core::ImageBuffer::size,
             core::ImageBuffer::size,
             QImage::Format_Indexed8)
{
    for(int i=0; i<256; ++i)
    {
        _image.setColor(i, qRgb(i, i, i));
    }

    _image.fill(0);
    setPixmap(QPixmap::fromImage(_image));
}

//------------------------------------------------------------------------------

void SnapshotWidget::update()
{
    const int size = core::ImageBuffer::size;

    uchar *pixels = _image.bits();
    core::gImageBuffer.fill(pixels);

    for(int i=0; i<256; ++i)
        _histogramData[i] = 0;

    for(int i=0; i<size*size; ++i)
    {
        ++_histogramData[pixels[i]];
    }

    setPixmap(QPixmap::fromImage(_image));
}

//------------------------------------------------------------------------------

}

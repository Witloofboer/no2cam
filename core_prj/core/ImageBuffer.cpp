#include "ImageBuffer.h"

#include <cstring>

#include <QMutexLocker>

namespace core {

//------------------------------------------------------------------------------

ImageBuffer::ImageBuffer()
    : _lock(new QMutex)
{
    uchar const * end = _buffer+size*size;
    for(uchar* pt = _buffer; pt < end; *pt++=0);
}

//------------------------------------------------------------------------------

static const int factor = BaseCamera::size / ImageBuffer::size;

void ImageBuffer::set(const BaseCamera::Snapshot snapshot)
{
    QMutexLocker lock(_lock);

    uchar *pt=_buffer;

    for(int i=0; i<ImageBuffer::size; ++i)
        for(int j=0; j<ImageBuffer::size; ++j)
        {
            *pt++ = snapshot[factor*j][factor*i] >> 8;
        }
}

//------------------------------------------------------------------------------

void ImageBuffer::fill(uchar *pixels)
{
    QMutexLocker lock(_lock);

    std::memcpy(pixels, _buffer, size*size);
}

//------------------------------------------------------------------------------

ImageBuffer gImageBuffer;

//------------------------------------------------------------------------------

}

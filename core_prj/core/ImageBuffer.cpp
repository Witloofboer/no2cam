#include "ImageBuffer.h"

#include <cstring>

#include <QMutexLocker>

#include "Snapshot.h"

namespace core {

//------------------------------------------------------------------------------

ImageBuffer::ImageBuffer()
    : _lock(new QMutex)
{
    uchar const * end = _buffer+size*size;
    for(uchar* pt = _buffer; pt < end; *pt++=0);
}

//------------------------------------------------------------------------------

static const int factor = snapshotSize / ImageBuffer::size;

void ImageBuffer::set(const Snapshot& snapshot)
{
    QMutexLocker lock(_lock);

    uchar *pt=_buffer;
    for(int i=0, ix=0; i<ImageBuffer::size; ++i, ix+=factor)
    {
        for(int j=0, jx=0; j<ImageBuffer::size; ++j, jx+=factor)
        {
            *pt++ = snapshot[jx][ix] >> 8;
        }
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

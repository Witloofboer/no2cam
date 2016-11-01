#ifndef IMAGEBUFFER_H
#define IMAGEBUFFER_H

class QMutex;

#include "BaseCamera.h"

namespace core {

//------------------------------------------------------------------------------

class CORESHARED_EXPORT ImageBuffer
{
public:
    static const int size=256;

    ImageBuffer();

    void set(const BaseCamera::Snapshot snapshot);
    void fill(uchar *pixels);

private:
    QMutex *_lock;
    uchar  _buffer[size*size];

};

//------------------------------------------------------------------------------

extern ImageBuffer CORESHARED_EXPORT gImageBuffer;

//------------------------------------------------------------------------------

}

#endif // IMAGEBUFFER_H

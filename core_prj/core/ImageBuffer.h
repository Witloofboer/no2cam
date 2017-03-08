#ifndef IMAGEBUFFER_H
#define IMAGEBUFFER_H

#include "core_global.h"
#include "Snapshot.h"

class QMutex;

namespace core {

//------------------------------------------------------------------------------

class CORESHARED_EXPORT ImageBuffer
{
public:
    static const int size=256;

    ImageBuffer();

    void set(const Snapshot &snapshot);
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

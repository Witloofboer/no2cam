#ifndef MOCKCAMERA_H
#define MOCKCAMERA_H

#include <QtGlobal>

class MockCamera
{
public:
    MockCamera();

private:
    qreal _images[9][2048][2048];
};

#endif // MOCKCAMERA_H

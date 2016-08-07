#ifndef MOCKCAMERA_H
#define MOCKCAMERA_H

#include <QtGlobal>

class MockCamera
{
public:
    MockCamera();

private:
    int _index;
    qreal _images[3][3][2048][2048];
};

#endif // MOCKCAMERA_H

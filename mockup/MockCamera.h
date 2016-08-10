#ifndef MOCKCAMERA_H
#define MOCKCAMERA_H

#include <QtGlobal>

class QTimer;

#include "AbstractCamera.h"

class MockCamera: public core::AbstractCamera
{
    Q_OBJECT

public:
    MockCamera();

    void takeSnapshot() override;
    void stop() override;

    int snapshot[2048][2048];

private slots:
    void snapshotRdyImpl();

private:
    QTimer *_timer;
    double _scene[2048][2048];
};

#endif // MOCKCAMERA_H

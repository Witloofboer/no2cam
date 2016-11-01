#ifndef MOCKCAMERA_H
#define MOCKCAMERA_H

#include <QtGlobal>

class QTimer;

#include "core/BaseCamera.h"

class MockCamera: public core::BaseCamera
{
    Q_OBJECT

public:
    MockCamera();

    void takeSnapshot() override;
    void stop() override;

    void copySnapshot(Snapshot &buffer) override;

private slots:
    void snapshotRdyImpl();

private:
    QTimer *_timer;
    Snapshot _scene;
    int _shift;
};

#endif // MOCKCAMERA_H

#ifndef MOCKCAMERA_H
#define MOCKCAMERA_H

#include <QtGlobal>

class QTimer;

#include "core/AbstractCamera.h"

class MockCamera: public core::AbstractCamera
{
    Q_OBJECT

public:
    MockCamera();

    void takeSnapshot() override;
    void stop() override;

    void copySnapshot(core::Snapshot& buffer) override;

private slots:
    void snapshotRdyImpl();

private:
    QTimer *_timer;
    core::Snapshot _scene;
    int _shift;
};

#endif // MOCKCAMERA_H

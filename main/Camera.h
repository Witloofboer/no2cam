#ifndef MOCKCAMERA_H
#define MOCKCAMERA_H

#include <QtGlobal>
#include "windows.h"
#include "dcamapi.h"
#include "qtimer.h"
#include "QTime"
#include "core/Snapshot.h"

class QTimer;

#include "core/BaseDrivers.h"

class HamamatsuCamera: public core::BaseCameraDriver
{
    Q_OBJECT

public:
    HamamatsuCamera(HDCAM hdcam);
    ~HamamatsuCamera();

    static BaseCameraDriver *getCamera();

    void setExposure(int exposure) override;
    void takeSnapshot() override;
    void stop() override;

    int my_dcamprop_getvalue( HDCAM _hdcam, int32 idprop, int32& lValue );

private slots:
    void checkFrameReady();

private:
    const HDCAM _hdcam;
    DCAM_FRAME	_frame;
    QTimer *_timer;
    core::Snapshot _snap;
};



#endif // MOCKCAMERA_H

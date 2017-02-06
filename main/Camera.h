#ifndef MOCKCAMERA_H
#define MOCKCAMERA_H

#include <QtGlobal>
#include "windows.h"
#include "dcamapi.h"


class QTimer;

#include "core/drivers.h"

class HamamatsuCamera: public core::CameraDriver
{
    Q_OBJECT

public:
    HamamatsuCamera();
    ~HamamatsuCamera();

    void setExposure(int exposure) override;
    void takeSnapshot() override;
    void stop() override;
    void getSnapshot(core::Snapshot &buffer) override;

    void init();
    void uninit();

    int my_dcamprop_getvalue( HDCAM _hdcam, int32 idprop, int32& lValue );

private:
    DCAMWAIT_OPEN	waitopen;
    DCAM_FRAME	frame;
    DCAMWAIT_START	paramwait;
    DCAMERR	_dcamErr;
    HDCAM _hdcam;
};



#endif // MOCKCAMERA_H

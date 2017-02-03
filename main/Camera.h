#ifndef MOCKCAMERA_H
#define MOCKCAMERA_H

#include <QtGlobal>
#include "windows.h"
#include "dcamapi.h"


class QTimer;

#include "core/BaseCamera.h"

class HamamatsuCamera: public core::BaseCamera
{
    Q_OBJECT

public:
    HamamatsuCamera();
    ~HamamatsuCamera();

    void takeSnapshot() override;
    void stop() override;

    void copySnapshot(Snapshot &buffer) override;

    void init();
    void uninit();
    void setExposure(int exposure) override;

    int my_dcamprop_getvalue( HDCAM _hdcam, int32 idprop, int32& lValue );


private:
    DCAMWAIT_OPEN	waitopen;
    DCAM_FRAME	frame;
    DCAMWAIT_START	paramwait;
    DCAMERR	_dcamErr;
    HDCAM _hdcam;
};



#endif // MOCKCAMERA_H

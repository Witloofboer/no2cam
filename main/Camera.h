#ifndef MOCKCAMERA_H
#define MOCKCAMERA_H

#include <QtGlobal>
#include "windows.h"
#include "dcamapi.h"
#include "qtimer.h"


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


    bool init();
    void uninit();

    int my_dcamprop_getvalue( HDCAM _hdcam, int32 idprop, int32& lValue );

private slots:
    void checkFrameReady();

private:
    DCAMWAIT_OPEN	waitopen;
    DCAM_FRAME	frame;
    DCAMWAIT_START	paramwait;
    DCAMERR	_dcamErr;
    HDCAM _hdcam;
    QTimer *_timer;
    int _exposureTime;
    bool state;
};



#endif // MOCKCAMERA_H

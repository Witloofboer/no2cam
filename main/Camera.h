#ifndef MOCKCAMERA_H
#define MOCKCAMERA_H

#include <QtGlobal>
#include "windows.h"
#include "dcamapi.h"
#include "qtimer.h"
#include "QTime"
#include "core/Snapshot.h"

class QTimer;

#include "core/drivers.h"

class HamamatsuCamera: public core::CameraDriver
{
    Q_OBJECT

public:
    HamamatsuCamera();
    ~HamamatsuCamera();

    bool init();

    void setExposure(int exposure) override;
    void takeSnapshot() override;
    void stop() override;

    int my_dcamprop_getvalue( HDCAM _hdcam, int32 idprop, int32& lValue );

private slots:
    void checkFrameReady();

private:
    QTime *time;
    DCAMWAIT_OPEN	waitopen;
    DCAM_FRAME	frame;
    DCAMWAIT_START	paramwait;
    DCAMERR	_dcamErr;
    HDCAM _hdcam;
    QTimer *_timer;
    bool state;
    _DWORD	dw = DCAMCAP_EVENT_FRAMEREADY;
    core::Snapshot _snap;
    //bool first_run = true;



};



#endif // MOCKCAMERA_H

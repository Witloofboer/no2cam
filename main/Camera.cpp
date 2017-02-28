#include "Camera.h"
#include <QMessageBox>
#include <QString>
#include "dcamprop.h"

//------------------------------------------------------------------------------

HamamatsuCamera::HamamatsuCamera()
    : core::CameraDriver()
    , _timer(new QTimer(this))
{
    _timer->setSingleShot(true);
    connect(_timer, QTimer::timeout, this, checkFrameReady);
}

//------------------------------------------------------------------------------

HamamatsuCamera::~HamamatsuCamera() {}

//------------------------------------------------------------------------------

bool HamamatsuCamera::init()
{
    // first initialize the DCAM API software
    DCAMAPI_INIT paraminit;
    memset( &paraminit, 0, sizeof(paraminit) );
    paraminit.size	= sizeof(paraminit);

    _dcamErr = dcamapi_init( &paraminit );

    if( failed( _dcamErr ) )
    {
        qInfo("CAM ERROR ON INIT: %d", _dcamErr); // TODO Error
        return false;
    }

    // once the software is initilized try to open the connected Camera
    qDebug("NR of Counted devices: %d", static_cast<int>(paraminit.iDeviceCount));

    DCAMDEV_OPEN	paramopen;
    memset( &paramopen, 0, sizeof(paramopen) );
    paramopen.size	= sizeof(paramopen);
    paramopen.index	= 0;
    _dcamErr = dcamdev_open( &paramopen );
    qInfo("debug: %x",_dcamErr);

    if( failed( _dcamErr ) )
    {
        qInfo("CAM ERROR ON DEV OPEN: %d", _dcamErr);
        dcamapi_uninit();
        return false;
    }

    // once the camera could be opened succesfully, couple the device ID to the device handler
    qDebug("INIT DCAM SUCCESFULLY!:");
    _hdcam = paramopen.hdcam;

    // set the frame variable and allocate 1 frame to the camera
    memset( &frame, 0, sizeof(frame) );
    frame.size	= sizeof(frame);
    frame.iFrame= -1;		// latest frame

    _dcamErr = dcamprop_setvalue(_hdcam, DCAM_IDPROP_BINNING, DCAMPROP_BINNING__2);
    qInfo("debug: %x",_dcamErr);

    //state = dcam_allocframe(_hdcam, 1);
    _dcamErr = dcambuf_alloc(_hdcam,1);
    qInfo("debug: %x",_dcamErr);

    // allow the software to send the trigger pulse to the camera
    _dcamErr = dcamprop_setvalue(_hdcam, DCAM_IDPROP_TRIGGERSOURCE, DCAMPROP_TRIGGERSOURCE__SOFTWARE );
    qInfo("debug: %x",_dcamErr);

    // set the timer as fast as possible
    _timer->setInterval(0);



    //start the capture mode of the camera (exposure will begin once camera receives trigger from software)
     _dcamErr = dcamcap_start( _hdcam, DCAMCAP_START_SEQUENCE );
    qInfo("debug: %x",_dcamErr);

    return true;
}

void HamamatsuCamera::checkFrameReady()
{
    _dcamErr = dcambuf_lockframe(_hdcam, &frame);
    emit snapshotAvailable();
}

//------------------------------------------------------------------------------

void HamamatsuCamera::uninit()
{
    dcambuf_release( _hdcam );
    dcamdev_close( _hdcam );
    dcamapi_uninit();
}

//------------------------------------------------------------------------------

void HamamatsuCamera::setExposure(int exposure)
{
    //qInfo("<exposure time: %d ms>", exposure);
    _dcamErr = dcamprop_setvalue(_hdcam, DCAM_IDPROP_EXPOSURETIME, ((double)exposure)/1000 );
    _exposureTime = exposure;
}

//------------------------------------------------------------------------------

void HamamatsuCamera::takeSnapshot()
{
    qInfo("<snapshotting>");
    state = dcam_firetrigger(_hdcam);
    time->restart();
    state = dcam_wait(_hdcam,&dw,1000,NULL);
    qInfo("time to take snapshot (ms): %d",time->elapsed());
    _timer->start();
}

//------------------------------------------------------------------------------

void HamamatsuCamera::stop()
{
    _timer->stop();
}

//------------------------------------------------------------------------------

void HamamatsuCamera::getSnapshot(core::Snapshot &buffer)
{
    memcpy(buffer, frame.buf, sizeof buffer);
}

//------------------------------------------------------------------------------

#include "Camera.h"
#include <QMessageBox>
#include <QString>
#include "dcamprop.h"
#include <stdlib.h>

//------------------------------------------------------------------------------

HamamatsuCamera::HamamatsuCamera(HDCAM hdcam)
    : core::BaseCameraDriver()
    , _hdcam(hdcam)
    , _timer(new QTimer(this))
{
    _timer->setSingleShot(true);
    connect(_timer, QTimer::timeout, this, checkFrameReady);

    // set the frame variable and allocate 1 frame to the camera
    memset(&_frame, 0, sizeof(_frame));
    _frame.size	= sizeof(_frame);
    _frame.iFrame= -1;		// latest frame
}

//------------------------------------------------------------------------------

HamamatsuCamera::~HamamatsuCamera()
{
    dcambuf_release(_hdcam);
    dcamdev_close(_hdcam);
    dcamapi_uninit();
}

//------------------------------------------------------------------------------

HamamatsuCamera *HamamatsuCamera::getCamera()
{
    // first initialize the DCAM API software
    DCAMAPI_INIT paraminit;
    memset(&paraminit, 0, sizeof(paraminit));
    paraminit.size= sizeof(paraminit);

    DCAMERR err = dcamapi_init(&paraminit);

    if (failed(err))
    {
        QMessageBox::critical(
            0, "Aborting",
            tr("<p>Camera init has failed.</p>"
               "<p>dcamapi_init error code: %1").arg(err));
        return nullptr;
    }

    if (paraminit.iDeviceCount != 1)
    {
        QMessageBox::critical(
            0, "Aborting",
            tr("<p>%1 camera(s) found. Expecting 1.</p>").arg(err));
        return nullptr;
    }

    DCAMDEV_OPEN paramopen;
    memset(&paramopen, 0, sizeof(paramopen));
    paramopen.size	= sizeof(paramopen);
    paramopen.index	= 0;
    err = dcamdev_open(&paramopen);

    if (failed(err))
    {
        QMessageBox::critical(
            0, "Aborting",
            tr("<p>Camera open has failed.</p>"
               "<p>dcamdev_open error code: %1").arg(err));
        dcamapi_uninit();
        return nullptr;
    }

    // once the camera could be opened succesfully, couple the device ID to the device handler
    HDCAM hdcam = paramopen.hdcam;


    err = dcamprop_setvalue(hdcam, DCAM_IDPROP_BINNING, DCAMPROP_BINNING__4);
    if (failed(err))
    {
        QMessageBox::critical(
            0, "Aborting",
            tr("<p>Camera binning setting has failed.</p>"
               "<p>dcamprop_setvalue error code: %1").arg(err));
        dcamdev_close(hdcam);
        dcamapi_uninit();
        return 0;
    }

    err = dcambuf_alloc(hdcam,1);
    if (failed(err))
    {
        QMessageBox::critical(
            0, "Aborting",
            tr("<p>Camera buffer allocation has failed.</p>"
               "<p>dcambuf_alloc error code: %1").arg(err));
        dcamdev_close(hdcam);
        dcamapi_uninit();
        return 0;
    }

    // allow the software to send the trigger pulse to the camera
    err = dcamprop_setvalue(hdcam, DCAM_IDPROP_TRIGGERSOURCE, DCAMPROP_TRIGGERSOURCE__SOFTWARE);
    if (failed(err))
    {
        QMessageBox::critical(
            0, "Aborting",
            tr("<p>Camera trigger setting has failed.</p>"
               "<p>dcamprop_setvalue error code: %1").arg(err));
        dcambuf_release(hdcam);
        dcamdev_close(hdcam);
        dcamapi_uninit();
        return 0;
    }

    //start the capture mode of the camera (exposure will begin once camera receives trigger from software)
    err = dcamcap_start(hdcam, DCAMCAP_START_SEQUENCE);
    if (failed(err))
    {
        QMessageBox::critical(
            0, "Aborting",
            tr("<p>Camera start has failed.</p>"
               "<p>dcamcap_start error code: %1").arg(err));
        dcambuf_release(hdcam);
        dcamdev_close(hdcam);
        dcamapi_uninit();
        return 0;
    }

    return new HamamatsuCamera(hdcam);
}

//------------------------------------------------------------------------------

void HamamatsuCamera::setExposure(int exposure)
{
    DCAMERR err = dcamprop_setvalue(_hdcam, DCAM_IDPROP_EXPOSURETIME, ((double)exposure)/1000);
    qDebug("<exposure time: %d ms (error code: %d)>", exposure, err);

    _timer->setInterval(exposure);
}

//------------------------------------------------------------------------------

void HamamatsuCamera::takeSnapshot()
{
    qDebug("<snapshotting>");
    dcam_firetrigger(_hdcam);
    _timer->start();
}

//------------------------------------------------------------------------------

void HamamatsuCamera::checkFrameReady()
{
    DWORD dw = DCAMCAP_EVENT_FRAMEREADY;
    dcam_wait(_hdcam, &dw, 100, NULL);
    dcambuf_lockframe(_hdcam, &_frame);

    core::Pixel *from = reinterpret_cast<core::Pixel *>(_frame.buf);
    for(int i=0; i<512; ++i)
        for(int j=0; j<512; ++j)
            _snap[i][j] = *from++;

    emit snapshotAvailable(_snap);
}

//------------------------------------------------------------------------------

void HamamatsuCamera::stop()
{
    qDebug("<camera stop>");
    _timer->stop();

    dcam_idle(_hdcam); // back to idle mode
    qDebug("Camera back to IDLE");

    DCAMERR err = dcamcap_start(_hdcam, DCAMCAP_START_SEQUENCE);
    qDebug("Camera back to CAPTURE, error code = %x", err);
}

//------------------------------------------------------------------------------

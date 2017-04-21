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
        qCritical("No camera detected (dcamapi_init error code: %d)", err);

        QMessageBox::critical(
            0, "Aborting",
            tr("<p><b>Failure</b>: no camera detected</p>"
               "<p>(dcamapi_init error code: %1)</p>").arg(err));
        return nullptr;
    }

    if (paraminit.iDeviceCount != 1)
    {
        qCritical("Multiple (%ld) cameras detected", paraminit.iDeviceCount);

        QMessageBox::critical(
            0,
            "Aborting",
            tr("<p><b>Failure</b>: more than 1 camera detected</p>"
               "<p>(%1 camera found, instead of only 1)</p>").arg(err));
        return nullptr;
    }

    DCAMDEV_OPEN paramopen;
    memset(&paramopen, 0, sizeof(paramopen));
    paramopen.size	= sizeof(paramopen);
    paramopen.index	= 0;
    err = dcamdev_open(&paramopen);

    if (failed(err))
    {
        qCritical("Error on camera opening (dcamdev_open error code: %d)", err);

        QMessageBox::critical(
            0,
            "Aborting",
            tr("<p><b>Failure</b>: camera not opened</p>"
               "<p>(dcamdev_open error code: %1)</p>").arg(err));
        dcamapi_uninit();
        return nullptr;
    }

    // once the camera could be opened succesfully, couple the device ID to the device handler
    HDCAM hdcam = paramopen.hdcam;


    err = dcamprop_setvalue(hdcam, DCAM_IDPROP_BINNING, DCAMPROP_BINNING__4);
    if (failed(err))
    {
        qCritical("Error on camera binning setting"
                  "(dcamprop_setvalue error code: %d)", err);

        QMessageBox::critical(
            0,
            "Aborting",
            tr("<p><b>Failure</b>: camera binning settings</p>"
               "<p>(dcamprop_setvalue error code: %1)</p>").arg(err));
        dcamdev_close(hdcam);
        dcamapi_uninit();
        return 0;
    }

    err = dcambuf_alloc(hdcam,1);
    if (failed(err))
    {
        qCritical("Error on camera buffer allocation"
                  "(dcambuf_alloc error code: %d)", err);

        QMessageBox::critical(
            0, "Aborting",
            tr("<p><b>Failure</b>: camera buffer allocation</p>"
               "<p>(dcambuf_alloc error code: %1)").arg(err));
        dcamdev_close(hdcam);
        dcamapi_uninit();
        return 0;
    }

    // allow the software to send the trigger pulse to the camera
    err = dcamprop_setvalue(hdcam,
                            DCAM_IDPROP_TRIGGERSOURCE,
                            DCAMPROP_TRIGGERSOURCE__SOFTWARE);
    if (failed(err))
    {
        qCritical("Error on camera trigger setting"
                  "(dcamprop_setvalue error code: %d)", err);

        QMessageBox::critical(
            0,
            "Aborting",
            tr("<p><b>Failure</b>: camera trigger settings.</p>"
               "<p>(dcamprop_setvalue error code: %1)</p>").arg(err));
        dcambuf_release(hdcam);
        dcamdev_close(hdcam);
        dcamapi_uninit();
        return 0;
    }

    // Start the capture mode of the camera
    // (exposure begins once camera receives trigger from software)
    err = dcamcap_start(hdcam, DCAMCAP_START_SEQUENCE);
    if (failed(err))
    {
        QMessageBox::critical(
            0, "Aborting",
            tr("<p><b>Failure</b>: camera starting</p>"
               "<p>(dcamcap_start error code: %1)</p>").arg(err));
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
    DCAMERR err = dcamprop_setvalue(_hdcam,
                                    DCAM_IDPROP_EXPOSURETIME,
                                    ((double)exposure)/1000);
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

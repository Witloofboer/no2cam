#ifndef BASECAMERA_H
#define BASECAMERA_H

#include <QObject>

#include "core_global.h"

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class CORESHARED_EXPORT BaseCamera : public QObject
{
    Q_OBJECT

public:
    static const int size = 2048;

    typedef quint16 Pixel;
    typedef Pixel Snapshot[size][size] ;

    explicit BaseCamera();

    void setExposure(int exposure);

    virtual void stop()=0;

    virtual void copySnapshot(Snapshot &buffer)=0;

signals:
    void snapshotAvailable();

public slots:
    virtual void takeSnapshot()=0;

protected:
    int _exposure;
};

//------------------------------------------------------------------------------

}

#endif // BASECAMERA_H

#ifndef ABSTRACTDRIVER_H
#define ABSTRACTDRIVER_H

#include <QObject>

#include "core_global.h"

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class CORESHARED_EXPORT AbstractDriver : public QObject
{
    Q_OBJECT

public:
    explicit AbstractDriver();
    virtual void stop()=0;
    virtual void setPower(double power)=0;
    virtual void cooldown()=0;
signals:

public slots:
};

//------------------------------------------------------------------------------

}

#endif // ABSTRACTDRIVER_H

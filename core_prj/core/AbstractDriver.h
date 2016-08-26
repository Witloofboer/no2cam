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

signals:

public slots:
};

//------------------------------------------------------------------------------

}

#endif // ABSTRACTDRIVER_H

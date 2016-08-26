#ifndef ABSTRACTGENERATOR_H
#define ABSTRACTGENERATOR_H

#include <QObject>

#include "core_global.h"

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class CORESHARED_EXPORT AbstractGenerator : public QObject
{
    Q_OBJECT

public:
    explicit AbstractGenerator();
};

//------------------------------------------------------------------------------

}

#endif // ABSTRACTGENERATOR_H

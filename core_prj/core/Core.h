#ifndef CORE_H
#define CORE_H

#include <QSettings>
#include <QThread>

#include "core_global.h"

class Core : public QThread
{
    Q_OBJECT
public:
    Core();
};

extern QSettings gSettings;
extern CORESHARED_EXPORT Core gCore;

#endif // CORE_H

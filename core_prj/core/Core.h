#ifndef CORE_H
#define CORE_H

#include <QObject>

#include "core_global.h"
#include "Crystal.h"

class CORESHARED_EXPORT Core : public QObject
{
    Q_OBJECT
public:
    Core();

public slots:

    /**
     * Requests the update of the parameters AOTF crystal.
     */
    void updateParameters();

private:
    Crystal aotfCrystal_;
};

extern CORESHARED_EXPORT Core gCore;

#endif // CORE_H

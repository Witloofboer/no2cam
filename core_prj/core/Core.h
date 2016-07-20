#ifndef CORE_H
#define CORE_H

#include <QObject>

#include "core_global.h"
#include "AotfCrystal.h"

class CORESHARED_EXPORT Core : public QObject
{
    Q_OBJECT
public:
    Core();

    AotfCrystal::Parameters getAotfCrystalParameters() const;

public slots:

    /**
     * Requests the update of the parameters AOTF crystal.
     */
    void updateAotfCrystalParameters(AotfCrystal::Parameters params);

private:
    AotfCrystal aotfCrystal_;
};

extern CORESHARED_EXPORT Core gCore;

#endif // CORE_H

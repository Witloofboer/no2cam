#include "Core.h"

Core::Core()
    : QObject()
    , aotfCrystal_(this)
{
}

void Core::updateAotfCrystalParameters(AotfCrystal::Parameters params)
{
    aotfCrystal_.setParameters(params);
}

Core gCore;

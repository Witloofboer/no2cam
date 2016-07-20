#include "Core.h"

Core::Core()
    : QObject()
    , aotfCrystal_(this)
{
}

void Core::updateParameters()
{
    aotfCrystal_.updateParameters();
}

Core gCore;

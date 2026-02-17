#pragma once
// Minimal compatibility header for V1 EnergyATM90E36 driver (expects debug.h)
// In V2 we route these to Serial for simplicity.

#include <Arduino.h>

#ifndef DBUGS
#define DBUGS Serial
#endif

#ifndef DEBUG_BEGIN
#define DEBUG_BEGIN(speed) Serial.begin(speed)
#endif

#ifndef DBUGF
#define DBUGF(...) Serial.printf(__VA_ARGS__)
#endif

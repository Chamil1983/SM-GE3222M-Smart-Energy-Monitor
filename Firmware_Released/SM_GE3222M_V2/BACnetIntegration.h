#pragma once

#include <Arduino.h>

class BACnetIntegration {
public:
    static void initialize();
    static void update();
    static void setEnabled(bool enabled);
    static bool isEnabled();

private:
    static void updateDeviceMetadataAndVendorProps();
    static void updateAnalogInputs();
    static void updateBinaryInputs();
    static void updateBinaryOutputs();
    static void applyBinaryOutputCommands();
    static void updateTimeSync();
    static float readScaledDCVoltage();
    static String maskedApPassword();

    static bool _enabled;
    static bool _initialized;
    static bool _ledRemoteOverrideEnable;
    static bool _exposeRawApPassword;
    static uint32_t _lastSyncMs;
    static uint32_t _lastMetaMs;
    static uint32_t _lastNtpAttemptMs;
    static bool _ntpInit;
    static uint32_t _lastNtpSyncUnix;
    static uint16_t _refreshIntervalMs;
    static float _dcDividerRatio;
    static float _dcGain;
    static float _dcOffset;
};


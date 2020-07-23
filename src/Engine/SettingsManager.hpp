#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <string>

enum class ShadowMode {
    NONE = 0,
    SINGLE_PASS = 1,
    ITERATE = 2,
    VARIANCE = 3
};

enum class BloomMode {
    NONE = 0,
    GAUSSIAN = 1,
    LINEAR_GAUSSIAN = 2
};

class UserSettings {
private:
    static std::string settingsPath();
    static void assignSetting( std::string setting, std::string value );

public:
    static void loadFromFile();
    static void writeToFile();

    // Assign default settings values
    static int resolution_width;
    static int resolution_height;
    
    static ShadowMode shadow_mode;
    static bool blur_shadow_map;
    static BloomMode bloom_mode;
    static bool use_volumetric_light_scattering;
    static bool use_FXAA;
    static bool use_exposure;
    static bool new_shading;
};

#endif
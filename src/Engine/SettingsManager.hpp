#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <string>

enum ShadowMode {
    NONE = 0,
    SINGLE_PASS = 1,
    ITERATE = 2
};

class UserSettings {
private:
    static void assignSetting( std::string setting, std::string value );

public:
    static void loadFromFile();

    // Assign default settings values
    static int resolution_width;
    static int resolution_height;
    
    static ShadowMode shadow_mode;
    static bool use_bloom;
    static bool use_volumetric_light_scattering;
};

#endif
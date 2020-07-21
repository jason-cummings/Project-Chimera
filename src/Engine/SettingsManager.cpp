#include "SettingsManager.hpp"

#include <fstream>
#include <iostream>

#include "Utilities/WAIWrapper.hpp"
#include "Utilities/Asset.hpp"

#define SETTINGS_FILENAME "settings.txt"

// Assign default settings values
int UserSettings::resolution_width = 2880;
int UserSettings::resolution_height = 1800;

ShadowMode UserSettings::shadow_mode = ShadowMode::ITERATE;
bool UserSettings::blur_shadow_map = false;
BloomMode UserSettings::bloom_mode = BloomMode::LINEAR_GAUSSIAN;
bool UserSettings::use_volumetric_light_scattering = true;
bool UserSettings::use_FXAA = true;
bool UserSettings::use_exposure = true;

// Return the path to the settings file
std::string UserSettings::settingsPath() {
    return WAIWrapper::getExecutablePath() + "/../../" + SETTINGS_FILENAME;
}

// Opens file at [exe path]/../../settings.txt and assigns all relevant settings
void UserSettings::loadFromFile() {
    std::string settings_path = settingsPath();
    std::ifstream settings_file( settings_path );
    if( !settings_file.is_open() ) {
        std::cerr << "Could not open settings file at " << settings_path << std::endl;
        writeToFile();
        return;
    }

    // Read the file line by line and assign settings
    std::string line;
    while( getline( settings_file, line ) ) {
        size_t delim_index = line.find( '=' );
        std::string setting = line.substr( 0, delim_index );
        std::string value = line.substr( delim_index+1 );
        assignSetting( setting, value );
    }

    settings_file.close();

    writeToFile();
}

void UserSettings::writeToFile() {
    std::string settings_path = settingsPath();
    std::ofstream settings_file( settings_path );

    settings_file << "resolution_width=" << resolution_width << std::endl;
    settings_file << "resolution_height=" << resolution_height << std::endl;
    settings_file << "shadow_mode=" << static_cast<int>(shadow_mode) << std::endl;
    settings_file << "blur_shadow_map=" << blur_shadow_map << std::endl;
    settings_file << "bloom_mode=" << static_cast<int>(bloom_mode) << std::endl;
    settings_file << "use_volumetric_light_scattering=" << use_volumetric_light_scattering << std::endl;
    settings_file << "use_FXAA=" << use_FXAA << std::endl;
    settings_file << "use_exposure=" << use_exposure << std::endl;

    settings_file.close();
}

// Assign the appropriate setting based on the values
void UserSettings::assignSetting( std::string setting, std::string value ) {
    if( setting == "resolution_width" ) {
        resolution_width = std::stoi(value);
    }
    else if( setting == "resolution_height" ) {
        resolution_height = std::stoi(value);
    }
    else if( setting == "shadow_mode" ) {
        shadow_mode = (ShadowMode)std::stoi(value);
    }
    else if( setting == "blur_shadow_map" ) {
        blur_shadow_map = (bool)std::stoi(value);
    }
    else if( setting == "bloom_mode" ) {
        bloom_mode = (BloomMode)std::stoi(value);
    }
    else if( setting == "use_volumetric_light_scattering" ) {
        use_volumetric_light_scattering = (bool)std::stoi(value);
    }
    else if( setting == "use_FXAA" ) {
        use_FXAA = (bool)std::stoi(value);
    }
    else if( setting == "use_exposure" ) {
        use_exposure = (bool)std::stoi(value);
    }
    else {
        std::cerr << "Unknown setting - " << setting << "=" << value << std::endl;
    }
}

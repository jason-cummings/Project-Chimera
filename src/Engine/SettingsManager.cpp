#include "SettingsManager.hpp"

#include <fstream>
#include <iostream>
#include <map>

#include "Utilities/WAIWrapper.hpp"
#include "Utilities/Asset.hpp"

#define SETTINGS_FILENAME "settings.txt"

// Assign default settings values
int UserSettings::resolution_width = 2880;
int UserSettings::resolution_height = 1800;

ShadowMode UserSettings::shadow_mode = ITERATE;
bool UserSettings::use_bloom = true;

// Opens file at [exe path]/../../settings.txt and assigns all relevant settings
void UserSettings::loadFromFile() {
    std::string settings_path = WAIWrapper::getExecutablePath() + "/../../" + SETTINGS_FILENAME;
    std::ifstream settings_file( settings_path );
    if( !settings_file.is_open() ) {
        std::cerr << "Could not open settings file at " << settings_path << std::endl;
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
    else if( setting == "use_bloom" ) {
        use_bloom = (bool)std::stoi(value);
    }
}

#include "MaterialFactory.hpp"

Material* MaterialFactory::createMaterial(fs::path input_directory, fs::path textures_folder){
    GLuint texture, emissive;
    float shininess;
    /*TO-DO Assign values to texture, emissive, and shininess.*/
    
    fs::path to_texture = input_directory; //To Materials subfolder
    to_texture.append("diffuse.txt"); 
    Asset* texture_asset = new Asset( to_texture ); 
    std::string texture_image = std::string( texture_asset->getBuffer() );

    fs::path new_to_texture = textures_folder;
    new_to_texture.append( texture_image ); 
    

    
    fs::path to_emissive = input_directory;
    to_emissive.append( "emissive.txt" );
    Asset* emissive_asset = new Asset( to_emissive );
    std::string emissive_image = std::string(emissive_asset->getBuffer());

    fs::path new_to_emissive = textures_folder;
    new_to_emissive.append( emissive_image );



    fs::path to_shininess = input_directory; 
    to_shininess.append("shininess");
    Asset* shininess_asset = new Asset( to_shininess );

    std::string texture_path_string = new_to_texture.string();
    std::string emissive_path_string = new_to_emissive.string();
    texture = TextureLoader::loadTexture( texture_path_string, false );
    emissive = TextureLoader::loadTexture( emissive_path_string, false );
    shininess = (float)strtod(shininess_asset->getBuffer(),NULL);
    
    Material *material = new Material( texture, emissive, shininess );
    return material;
}
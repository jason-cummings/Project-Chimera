#include "Material.hpp"

// Material Implementation

Material *Material::default_material = nullptr;

Material::Material(GLuint texture_, GLuint emissive_, float shininess_){
    texture = texture_;
    emissive = emissive_;
    shininess = shininess_;
}

void Material::bind(Shader *shader, bool use_shininess) {
    if( use_shininess ) {
        shader->setUniformFloat( "materialShininess", shininess );
    }

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture );
    glActiveTexture( GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_2D, emissive );

    shader->setUniformInt( "colorTexture", 0 );
    shader->setUniformInt( "emissiveTexture", 1 );
}

void Material::loadDefaultMaterial() { 
    // Get the paths for the default material
    fs::path default_path = Asset::assetPath();
    default_path.append( DEFAULT_MATERIAl_PATH );
    fs::path default_textures_path = Asset::assetPath();
    default_textures_path.append( DEFAULT_TEXTURES_PATH );
    default_material = MaterialFactory::createMaterial( default_path, default_textures_path );
}



// MaterialFactory Implementation

Material* MaterialFactory::createMaterial(fs::path input_directory){
    fs::path default_tex_path = Asset::assetPath();
    default_tex_path.append("Textures");
    return createMaterial( input_directory, default_tex_path );
}


Material* MaterialFactory::createMaterial(fs::path input_directory, fs::path textures_folder){
    GLuint texture, emissive;
    float shininess;
    
    fs::path to_texture = input_directory; //To Materials subfolder
    to_texture.append("diffuse.txt"); 
    Asset* texture_asset = new Asset( to_texture ); 
    if( texture_asset->getBuffer() == nullptr ) {
        return Material::getDefaultMaterial();
    }
    std::string texture_image = std::string( texture_asset->getBuffer() );

    fs::path new_to_texture = textures_folder;
    new_to_texture.append( texture_image ); 
    

    
    fs::path to_emissive = input_directory;
    to_emissive.append( "emissive.txt" );
    Asset* emissive_asset = new Asset( to_emissive );
    if( emissive_asset->getBuffer() == nullptr ) {
        return Material::getDefaultMaterial();
    }
    std::string emissive_image = std::string(emissive_asset->getBuffer());

    fs::path new_to_emissive = textures_folder;
    new_to_emissive.append( emissive_image );



    fs::path to_shininess = input_directory; 
    to_shininess.append("shininess");
    Asset* shininess_asset = new Asset( to_shininess );
    if( shininess_asset->getBuffer() == nullptr ) {
        return Material::getDefaultMaterial();
    }

    std::string texture_path_string = new_to_texture.string();
    std::string emissive_path_string = new_to_emissive.string();
    texture = TextureLoader::loadTexture( texture_path_string, false );
    emissive = TextureLoader::loadTexture( emissive_path_string, false );
    shininess = (float)strtod(shininess_asset->getBuffer(),NULL);
    
    Material *material = new Material( texture, emissive, shininess );
    return material;
}


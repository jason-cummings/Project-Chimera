#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../GameObject.hpp"

class Light: public GameObject {
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float linear_attenuation;
    float quadratic_attenuation;
    bool directional;

public:
    Light( std::string id );

    inline glm::vec3 getLocation() { return glm::vec3( world_transform[3] ); }
    inline glm::vec3 getAmbient() { return ambient; }
    inline glm::vec3 getDiffuse() { return diffuse; }
    inline glm::vec3 getSpecular() { return specular; }
    inline float getLinearAttenuation() { return linear_attenuation; }
    inline float getQuadraticAttenuation() { return quadratic_attenuation; }
    inline float getDirectional() { return directional; }

    inline void setAmbient( glm::vec3 val_in ) { ambient = val_in; }
    inline void setDiffuse( glm::vec3 val_in ) { diffuse = val_in; }
    inline void setSpecular( glm::vec3 val_in ) { specular = val_in; }
    inline void setLinearAttenuation( float val_in ) { linear_attenuation = val_in; }
    inline void setQuadraticAttenuation( float val_in ) { quadratic_attenuation = val_in; }
    inline void setDirectional( bool val_in ) { directional = val_in; }
};

#endif

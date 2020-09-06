#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "../GameObject.hpp"
#include <iostream>

// Value to use to calculate the effective radius of the point light
#define ATTENUATION_THRESHOLD .01f

class PointLight: public GameObject {
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float linear_attenuation = 1.0;
    float quadratic_attenuation = 1.0;

    // The effective radius is stored as the scale of the transform for easy rendering via the light's model matrix
    inline void calculateEffectiveRadius() {
        float a = ATTENUATION_THRESHOLD * quadratic_attenuation;
        float b = ATTENUATION_THRESHOLD * linear_attenuation;
        float c = ATTENUATION_THRESHOLD - 1.f;
        float effective_radius = (-b + sqrt(b*b - 4*a*c)) / (2*a);
        setScale( glm::vec3(effective_radius) );
    }

public:
    PointLight( std::string id ): GameObject(id) {}

    inline glm::vec3 getLocation() { return glm::vec3( world_transform[3] ); }
    inline glm::vec3 getAmbient() { return ambient; }
    inline glm::vec3 getDiffuse() { return diffuse; }
    inline glm::vec3 getSpecular() { return specular; }
    inline float getLinearAttenuation() { return linear_attenuation; }
    inline float getQuadraticAttenuation() { return quadratic_attenuation; }

    inline void setAmbient( glm::vec3 val_in ) { ambient = val_in; }
    inline void setDiffuse( glm::vec3 val_in ) { diffuse = val_in; }
    inline void setSpecular( glm::vec3 val_in ) { specular = val_in; }
    inline void setLinearAttenuation( float val_in ) { linear_attenuation = val_in; calculateEffectiveRadius(); }
    inline void setQuadraticAttenuation( float val_in ) { quadratic_attenuation = val_in; calculateEffectiveRadius(); }
};

#endif

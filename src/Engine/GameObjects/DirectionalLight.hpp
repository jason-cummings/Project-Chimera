#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include "../GameObject.hpp"

class DirectionalLight: public GameObject {
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float distance_thresholds[4] = { 5.f, 15.f, 40.f, 100.f };
    std::vector<glm::mat4> proj_matrices;

    // Maintain these as void * pointers so that the Light itself never tries to do anything with them
    // These should only be set and modified by rendersystem when registered
    void *depth_framebuffer;
    void *shadow_framebuffer;

public:
    DirectionalLight( std::string id );

    inline glm::vec3 getLocation() { return glm::vec3( world_transform[3] ); }
    inline glm::vec3 getAmbient() { return ambient; }
    inline glm::vec3 getDiffuse() { return diffuse; }
    inline glm::vec3 getSpecular() { return specular; }
    inline float getDistanceThreshold( size_t i ) { return distance_thresholds[i]; }
    inline glm::mat4 getProjectionMatrix( size_t i ) { return proj_matrices[i]; }
    inline void * getDepthFramebuffer() { return depth_framebuffer; }
    inline void * getShadowFramebuffer() { return shadow_framebuffer; }

    inline void setAmbient( glm::vec3 val_in ) { ambient = val_in; }
    inline void setDiffuse( glm::vec3 val_in ) { diffuse = val_in; }
    inline void setSpecular( glm::vec3 val_in ) { specular = val_in; }
    inline void setDepthFramebuffer( void *to_set ) { depth_framebuffer = to_set; }
    inline void setShadowFramebuffer( void *to_set ) { shadow_framebuffer = to_set; }
};

#endif

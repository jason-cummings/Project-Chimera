#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./../GameObject.hpp"

class Camera : public GameObject{
public:
    Camera();
    ~Camera();
    void updateCamera( float xchange, float ychange );
    void reshape( int new_width, int new_height ); 
    void createMatrices();

    // Getters to be used by render system
    inline glm::mat4 getViewMatrix() { return view_mat; }
    inline glm::mat4 getProjectionMatrix() { return proj_mat; }

    inline int getViewWidth() { return view_width; }
    inline int getViewHeight() { return view_height; }

    inline float getTh() { return th; }
    inline float getPh() { return ph; }

    inline float getOffset() { return offset; }
    inline void setOffset( float input ) { offset = input; }

private:
    glm::mat4 view_mat;
    glm::mat4 proj_mat;
    float th, ph ;
    int view_width, view_height;
    float aspect_ratio, fov = 55.f;
    float offset; //Nothing done with offset as of yet
};






#endif
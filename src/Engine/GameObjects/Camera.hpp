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
    void updateCamera(glm::vec3 change);
    void reshape( int new_width, int new_height ); 
private:
    glm::mat4 view_mat;
    glm::mat4 proj_mat;
    float th, ph ;
    int view_width, view_height;
    float aspect_ratio, fov = 55.f;
    void createMatrices();

};






#endif
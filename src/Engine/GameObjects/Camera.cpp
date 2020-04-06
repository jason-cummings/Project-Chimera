#include "Camera.hpp"

Camera::Camera() : GameObject(1){
    th = 0;
    ph = 0;
    //createMatrices();
}

Camera::~Camera(){
}

void Camera::updateCamera( glm::vec3 change ){

}

void Camera::reshape(int new_width, int new_height){
    view_width = new_width;
	view_height = new_height;
	aspect_ratio = view_width / (float)view_height;
}

void Camera::createMatrices(){
    float offset = 10;
    glm::vec3 viewpos = glm::vec3( parent->getWorldTransform() * glm::vec4(0.f,0.f,0.f,1.f) );
    glm::vec3 eyepos = viewpos + offset * glm::vec3(sin(th) * cos(ph), sin(ph), cos(th) * cos(ph));
    view_mat = glm::lookAt( eyepos, glm::vec3(viewpos), glm::vec3(0.f,1.f,0.f) );
    proj_mat = glm::perspective(glm::radians(fov), aspect_ratio , 0.1f, 100.f);
}
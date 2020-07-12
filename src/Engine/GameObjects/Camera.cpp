#include "Camera.hpp"

#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(): GameObject("Camera") {
    th = 0;
    ph = 0;
    //createMatrices();
}

Camera::~Camera() {

}

//Updates camera's th and ph when mouse is moved
void Camera::updateCamera(float xchange, float ychange) {
    th += xchange;
    ph += ychange;

    //Stops camera from inverting when going above or below player model
    if( ph > 1.57 ) {
        ph = 1.57f;
    }
    else if( ph < -1.57 ) {
        ph = -1.57f;
    }
    // Bound th as well just for overflow prevention
    if( th < -3.141592 ) {
        th += 6.283184f;
    }
    else if( th > 3.141592 ) {
        th -= 6.283184f;
    }
}

void Camera::reshape(int new_width, int new_height) {
    view_width = new_width;
	view_height = new_height;
	aspect_ratio = view_width / (float)view_height;
}

void Camera::createMatrices() {
    float offset = 3;
    glm::vec3 viewpos = glm::vec3( parent->getWorldTransform() * glm::vec4(0.f,1.f,0.f,1.f) );
    glm::vec3 eyepos = viewpos + offset * glm::vec3(sin(th) * cos(ph), sin(ph), cos(th) * cos(ph));
    glm::vec3 updir = glm::vec3( 0.f, 1.f, 0.f );

    view_mat = glm::lookAt( eyepos, viewpos, updir );
    proj_mat = glm::perspective( glm::radians(fov), aspect_ratio , 1.f, 10000.f );
}

glm::vec3 Camera::getEyePos() {
    float offset = 3;
    glm::vec3 viewpos = glm::vec3( parent->getWorldTransform() * glm::vec4(0.f,1.f,0.f,1.f) );
    return viewpos + offset * glm::vec3(sin(th) * cos(ph), sin(ph), cos(th) * cos(ph));
}

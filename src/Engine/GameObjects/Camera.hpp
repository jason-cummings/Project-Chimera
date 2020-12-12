#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/mat4x4.hpp>

#include "./../GameObject.hpp"

class Camera : public GameObject {
  public:
    Camera();
    ~Camera();

    void modifyAngles(float d_th, float d_ph);
    void setAngles(float new_th, float new_ph);

    void setResolution(int new_width, int new_height);
    void createMatrices();

    // Getters to be used by render system
    inline glm::mat4 getViewMatrix() { return view_mat; }
    inline glm::mat4 getProjectionMatrix() { return proj_mat; }
    glm::vec3 getEyePos();

    inline float getTh() { return th; }
    inline float getPh() { return ph; }

    inline float getOffset() { return offset; }
    inline void setOffset(float input) { offset = input; }

  private:
    float th, ph;
    float aspect_ratio, fov = 55.f;
    float offset;

    glm::mat4 view_mat;
    glm::mat4 proj_mat;
};

#endif
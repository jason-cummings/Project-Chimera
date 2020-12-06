#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

enum class RenderableType {
    NONE,
    MESH,
    SKINNED_MESH,
    OVERLAY,
    PARTICLE_SYSTEM
};

class Renderable {
  public:
    virtual RenderableType getType() = 0;
};

#endif
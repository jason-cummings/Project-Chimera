#include "CastleLevel.hpp"

#include "../GameObjects/DynamicCube.hpp"

CastleLevel::CastleLevel(): Level("TowersOld") {}

void CastleLevel::populateLevel() {
    skybox = SkyboxFactory::getSkybox("Skyboxes/5Degrees");
    // goal = glm::vec3( 20.f, 40.f, -1.f );

    // Physics things
    glm::vec3 o1scale(.9f);
    glm::quat o1rot( glm::vec3(0.f, glm::radians(0.f), 0.f) );
    glm::vec3 o1trans(0.f, 3.f, 10.f);
    DynamicCube *o1 = new DynamicCube( o1scale.x );
    o1->setTransform( o1scale, o1rot, o1trans );
    
    glm::vec3 o2scale(.9f);
    glm::quat o2rot( glm::vec3(glm::radians(15.f), glm::radians(15.f), 0.f) );
    glm::vec3 o2trans(0.8f, 3.f, 10.f);
    DynamicCube *o2 = new DynamicCube( o2scale.x );
    o2->setTransform( o2scale, o2rot, o2trans );
    
    glm::vec3 o3scale(1.f);
    glm::quat o3rot( glm::vec3(glm::radians(40.f), glm::radians(15.f), glm::radians(25.f)) );
    glm::vec3 o3trans(-1.5f, 4.f, 10.f);
    DynamicCube *o3 = new DynamicCube( o3scale.x );
    o3->setTransform( o3scale, o3rot, o3trans );

    scene->addChild(o1);
    scene->addChild(o2);
    scene->addChild(o3);
}

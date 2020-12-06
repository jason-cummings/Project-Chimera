#include "CastleLevel.hpp"

#include "../GameObjects/DynamicCube.hpp"

CastleLevel::CastleLevel() : Level(CASTLE_LEVEL_NAME) {}

void CastleLevel::populateLevel() {
    skybox = SkyboxFactory::getSkybox("Skyboxes/NightSky");
    // goal = glm::vec3( 20.f, 40.f, -1.f );
    spawn_translation = glm::vec3(5.f, 0.f, 0.f);
    spawn_rotation = glm::quat(glm::vec3(0.f, glm::radians(-90.f), 0.f));

    // // Physics things
    // glm::vec3 o1scale(.9f);
    // glm::quat o1rot( glm::vec3(0.f, glm::radians(0.f), 0.f) );
    // glm::vec3 o1trans(0.f, 3.f, 10.f);
    // DynamicCube *o1 = new DynamicCube( o1scale.x );
    // o1->setTransform( o1scale, o1rot, o1trans );

    // glm::vec3 o2scale(.9f);
    // glm::quat o2rot( glm::vec3(glm::radians(15.f), glm::radians(15.f), 0.f) );
    // glm::vec3 o2trans(0.8f, 3.f, 10.f);
    // DynamicCube *o2 = new DynamicCube( o2scale.x );
    // o2->setTransform( o2scale, o2rot, o2trans );

    // glm::vec3 o3scale(1.f);
    // glm::quat o3rot( glm::vec3(glm::radians(40.f), glm::radians(15.f), glm::radians(25.f)) );
    // glm::vec3 o3trans(-1.5f, 4.f, 10.f);
    // DynamicCube *o3 = new DynamicCube( o3scale.x );
    // o3->setTransform( o3scale, o3rot, o3trans );

    // scene->addChild(o1);
    // scene->addChild(o2);
    // scene->addChild(o3);

    DirectionalLight *moon = new DirectionalLight("moon");
    moon->setTransform(glm::vec3(1.f), glm::vec3(0.f), glm::vec3(-.8f, 3.f, 1.f));
    moon->setAmbient(glm::vec3(0.1f, 0.1f, 0.15f) * .1f);
    moon->setDiffuse(glm::vec3(0.2f, 0.25f, 0.3f) * .2f);
    moon->setSpecular(glm::vec3(0.f, 0.f, 0.f));
    directional_lights.push_back(moon);

    // And in the other other last step, Jason said "Let there be yet another light"
    // PointLight *point = new PointLight("point");
    // point->setTransform( glm::vec3(1.f), glm::vec3(0.f), glm::vec3(-1.f,2.f,-.707f) );
    // point->setAmbient( glm::vec3(0.0f,0.0f,0.0f) );
    // point->setDiffuse( glm::vec3(1.f,0.2f,0.8f) );
    // point->setSpecular( glm::vec3(0.f,0.f,0.f) );
    // point->setLinearAttenuation( 0.03f );
    // point->setQuadraticAttenuation( 0.02f );
    // point_lights.push_back( point );

    GameObject *coals_object = scene->getGameObject("coals");
    PointLight *c_point = new PointLight("coal_light");
    c_point->setTransform(glm::vec3(1.f), glm::vec3(0.f), glm::vec3(coals_object->getWorldTransform()[3]));
    c_point->setAmbient(glm::vec3(0.0f, 0.0f, 0.0f));
    c_point->setDiffuse(glm::vec3(1.f, 0.2f, 0.0f));
    c_point->setSpecular(glm::vec3(0.f, 0.f, 0.f));
    c_point->setLinearAttenuation(0.0f);
    c_point->setQuadraticAttenuation(0.7f);
    point_lights.push_back(c_point);
}

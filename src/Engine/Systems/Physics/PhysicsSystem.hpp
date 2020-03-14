#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <map>
#include <iostream>
#include <btBulletDynamicsCommon.h>

#include "../../GameObject.hpp"
#include "PhysicsComponent.hpp"

#define DEFAULT_GRAVITY -9.8 // m/s ideally

class PhysicsSystem {
private:
	// Bullet setup variables
	btDefaultCollisionConfiguration* collision_configuration;  // Contains default memory and collision setup
	btCollisionDispatcher* dispatcher;                  	   // Collision dispatcher
	btBroadphaseInterface* overlapping_pair_cache;             // Solver for the broad phase
	btSequentialImpulseConstraintSolver* solver;               // The default constraint solver
	btDiscreteDynamicsWorld* dynamics_world;                   // The dynamics world to be stepped

    // Components
    std::map<int, PhysicsComponent*> components;

public:
    PhysicsSystem();
    ~PhysicsSystem();

    // Add a component if one does not already exist with the same ID
    void addComponent( PhysicsComponent *new_comp );

    // Step through GameObject hierarchy and add all physics components
    void addSceneComponents( GameObject *obj );

    // Step the physics simulation 
    void stepPhysics( double dt );
};

#endif
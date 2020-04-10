#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <map>
#include <iostream>
#include <btBulletDynamicsCommon.h>

#include "../../GameObject.hpp"
#include "RigidBodyPhysicsComponent.hpp"

#define DEFAULT_GRAVITY -9.8 // m/s ideally

class PhysicsSystem {
private:
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	// Bullet setup variables
	btDefaultCollisionConfiguration* collision_configuration;  // Contains default memory and collision setup
	btCollisionDispatcher* dispatcher;                  	   // Collision dispatcher
	btBroadphaseInterface* overlapping_pair_cache;             // Solver for the broad phase
	btSequentialImpulseConstraintSolver* solver;               // The default constraint solver
	btDiscreteDynamicsWorld* dynamics_world;                   // The dynamics world to be stepped

    // Components
    std::map<std::string, RigidBodyPhysicsComponent*> components;

public:
    PhysicsSystem();
    ~PhysicsSystem();

    // Add a component if one does not already exist with the same ID
    void addRigidBody( RigidBodyPhysicsComponent *new_comp );

    // Step through GameObject hierarchy and add all physics components
    void addSceneComponents( GameObject *obj );

    // Step the physics simulation 
    void stepPhysics( double dt );

    // Perform a closest ray cast test in the physics world and return the resulting struct 
    void closestRayCast( btVector3 from, btVector3 to, btCollisionWorld::ClosestRayResultCallback &callback );
};

#endif
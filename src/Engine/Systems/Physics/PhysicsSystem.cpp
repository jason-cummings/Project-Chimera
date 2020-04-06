#include "PhysicsSystem.hpp"

PhysicsSystem::PhysicsSystem() {
	// Collision configuration contains default setup for memory, collision setup
	collision_configuration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_configuration); // Use the default collision dispatcher
	overlapping_pair_cache = new btDbvtBroadphase(); // btDbvtBroadphase is a good general purpose broadphase
    solver = new btSequentialImpulseConstraintSolver; // The default constraint solver

    // Set up the world and add some gravity
	dynamics_world = new btDiscreteDynamicsWorld(dispatcher, overlapping_pair_cache, solver, collision_configuration);
	dynamics_world->setGravity(btVector3(0, btScalar(DEFAULT_GRAVITY), 0)); //DEFAULT_GRAVITY
}

PhysicsSystem::~PhysicsSystem() {
    for( int i=dynamics_world->getNumCollisionObjects()-1; i>=0; i-- ) {
		btCollisionObject* obj = dynamics_world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if( body && body->getMotionState() ) {
			delete body->getMotionState();
		}
		dynamics_world->removeCollisionObject(obj);
		delete obj;
	}

    delete collision_configuration;
    delete dispatcher;
    delete overlapping_pair_cache;
    delete solver;
    delete dynamics_world;
}

// Add a component if one does not already exist with the same ID
void PhysicsSystem::addRigidBody( RigidBodyPhysicsComponent *new_comp ) {
    // if( components.count( new_comp->getID() ) > 0 ) {
    //     std::cerr << "Could not add Physics Component: A PhysicsComponent with id " << new_comp->getID() << " already exists in PhysicsSystem" << std::endl;
    // } 
    // else { 
        components[new_comp->getID()] = new_comp;
        dynamics_world->addRigidBody( new_comp->getCollisionObject() );
    // }
}

// Step through GameObject hierarchy and add all physics components
void PhysicsSystem::addSceneComponents( GameObject *obj ) {
    if( obj->hasPhysicsComponent() ) {
        addRigidBody( obj->getPhysicsComponent() );
    }
    for( int i=0; i<obj->getNumChildren(); i++ ) {
        addSceneComponents( obj->getChild(i) );
    }
}

// Step the physics simulation by time dt
void PhysicsSystem::stepPhysics( double dt ) {
    dynamics_world->stepSimulation( btScalar(dt), 10 );
}

#include "PhysicsSystem.hpp"

PhysicsSystem::PhysicsSystem() {
	// Collision configuration contains default setup for memory, collision setup
	collision_configuration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_configuration); // Use the default collision dispatcher
	overlapping_pair_cache = new btDbvtBroadphase(); // btDbvtBroadphase is a good general purpose broadphase
    solver = new btSequentialImpulseConstraintSolver; // The default constraint solver

    // Set up the world and add some gravity
	dynamics_world = new btDiscreteDynamicsWorld(dispatcher, overlapping_pair_cache, solver, collision_configuration);
	dynamics_world->setGravity(btVector3(0, DEFAULT_GRAVITY, 0));


    // bullet 3 HelloWorld example
    {
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -56, 0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamics_world->addRigidBody(body);
	}

    {
        //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
        btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		collisionShapes.push_back(colShape);

        /// Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();

        btScalar mass(1.f);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            colShape->calculateLocalInertia(mass, localInertia);

        startTransform.setOrigin(btVector3(2, 10, 0));

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        dynamics_world->addRigidBody(body);
    }
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
void PhysicsSystem::addComponent( PhysicsComponent *new_comp ) {
    if( components.count( new_comp->getID() ) > 0 ) {
        std::cerr << "Could not add Physics Component: A PhysicsComponent with id " << new_comp->getID() << " already exists in PhysicsSystem" << std::endl;
    } 
    else { 
        components[new_comp->getID()] = new_comp;
        dynamics_world->addCollisionObject( new_comp->getCollisionObject() );
    }
}

// Step through GameObject hierarchy and add all physics components
void PhysicsSystem::addSceneComponents( GameObject *obj ) {
    if( obj->hasPhysicsComponent() ) {
        addComponent( obj->getPhysicsComponent() );
    }
    for( int i=0; i<obj->getNumChildren(); i++ ) {
        addSceneComponents( obj->getChild(i) );
    }
}

// Step the physics simulation by time dt
void PhysicsSystem::stepPhysics( double dt ) {
    dynamics_world->stepSimulation( btScalar(dt), 10 );
}

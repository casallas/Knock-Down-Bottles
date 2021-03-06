/*
 *  BDScene.cpp
 *  Boeing Demo
 *
 *  Created by WATCH on 8/18/09.
 *  Copyright 2009 Iowa State University. All rights reserved.
 *
 */

#include "BDScene.h"

extern float _navSpeed;

BDScene::BDScene()
{
	// Create the device and network input controllers
	_deviceInputController = new DeviceInputController();

	// Initialize the audio manager and audio flags
	_isMaster = false;
	_totalTime = 0.0;
	_aimingVector = osg::Vec3(1, -7, -10);
	_mass = 3.0;
	
	// Register listening keys with KVReflector
	aq::KVReflector::instance()->addObserverWithKey(this, "Update_Wand_Matrix");
	aq::KVReflector::instance()->addObserverWithKey(this, "Decrease_Nav_Speed");
	aq::KVReflector::instance()->addObserverWithKey(this, "Increase_Nav_Speed");
	aq::KVReflector::instance()->addObserverWithKey(this, "Drop ball");
	aq::KVReflector::instance()->addObserverWithKey(this, "Aim_Left");
	aq::KVReflector::instance()->addObserverWithKey(this, "Aim_Right");
	aq::KVReflector::instance()->addObserverWithKey(this, "Aim_Up");
	aq::KVReflector::instance()->addObserverWithKey(this, "Aim_Down");
	aq::KVReflector::instance()->addObserverWithKey(this, "Mass_1");
	aq::KVReflector::instance()->addObserverWithKey(this, "Mass_2");
	aq::KVReflector::instance()->addObserverWithKey(this, "Mass_3");
	aq::KVReflector::instance()->addObserverWithKey(this, "Reset_Scene");
}

void BDScene::setMaster(bool isMaster)
{
	_isMaster = isMaster;
}

void BDScene::init()
{
	/*
	 
	 _rootNode			/-- _lightsGroup
			\          /             \
			 \-- _navTrans            \-- _lightSource1, 2, 3, 4
					  \							\
			           \						 \-- _light1, 2, 3, 4
				        \-- _models
							    \
				                 \-- ...
	 */

	_rootNode = new osg::Group();
	_navTrans = new osg::MatrixTransform();
	
	_models = new osg::Group();
	_wandTrans = new osg::MatrixTransform();
	
	_boxes = new osg::Group();
	_launchedObjects = new osg::Group();
	
	_models->addChild(_launchedObjects.get());
	_models->addChild(_boxes.get());
	
	// Set the models node to normal scaling
	_models->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
	_launchedObjects->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
	
	// Setup the scenegraph hierarchy
	_rootNode->addChild(_navTrans.get());
	_navTrans->addChild(_models.get());
	_models->addChild(_wandTrans.get());
	
	initPhysics();
	setupBoxes();
	
	// Initialize the lights group for KVO notifications
	_lightsGroup = new LightsGroup(_models->getOrCreateStateSet());
	
	// Add the lighting to the scene
	_lightsGroup->init();
	_navTrans->addChild(_lightsGroup.get());

	// Run optimization over the scene graph.
	// NOTE: Using the optimizer may cause problems with textures not showing
	// up on all walls.
	osgUtil::Optimizer optimizer;
	optimizer.optimize(_rootNode.get());
}

void BDScene::initPhysics()
{
	// setup the broadphase pass
	btVector3 worldAabbMin(-10000, -10000, -10000);
	btVector3 worldAabbMax(10000, 10000, 10000);
	int maxProxies = 1024;
//	btBroadphaseInterface *broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies); // better for more static scenes
	btBroadphaseInterface *broadphase = new btDbvtBroadphase(); // better for scenes with more movement
	
	btDefaultCollisionConfiguration *collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver;
	
	_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	_dynamicsWorld->setGravity(btVector3(0, -9.8, 0));
	
	btCollisionShape *groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
	btRigidBody::btRigidBodyConstructionInfo
	groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	_dynamicsWorld->addRigidBody(groundRigidBody);
	
	_models->addChild(createOSGBox(osg::Vec3(10000,.1,10000)));
}

void BDScene::setupBoxes()
{
	// make the box and collision shape once - that way bullet has less unique collision shapes
	// this is suggested in the bullet wiki
	float boxSize = 0.5;
	osg::ref_ptr<osg::MatrixTransform> node = createOSGBox(osg::Vec3(boxSize, boxSize, boxSize));
	btCollisionShape *cShape = osgbBullet::btBoxCollisionShapeFromOSG(node.get());	
	btScalar mass(30.0);
	btVector3 inertia;
	cShape->calculateLocalInertia(mass, inertia);
	
	for (int i=-10; i < 10; i++)
	{
		for (int j=0; j < 12; j++)
		{
			// put each box in its place
			osg::ref_ptr<osg::MatrixTransform> boxClone = createOSGBox(osg::Vec3(boxSize, boxSize, boxSize));
			osgbBullet::MotionState *motion = new osgbBullet::MotionState;
			motion->setTransform(boxClone.get());
			
			btTransform shapeTransform;
			shapeTransform.setIdentity();
			shapeTransform.setOrigin(btVector3(i, j+0.5, -5)); // change this to move the initial position of the object
			motion->setWorldTransform(shapeTransform);
			
			btRigidBody::btRigidBodyConstructionInfo rbinfo(mass, motion, cShape, inertia);
			btRigidBody *body = new btRigidBody(rbinfo);
			_dynamicsWorld->addRigidBody(body);
			
			_boxes->addChild(boxClone.get());
		}
	}
}

void BDScene::dropBall()
{
	std::cout << "Launching ball with axis " << _aimingVector.x() << ", " << _aimingVector.y() << ", " << _aimingVector.z() << std::endl;
	osg::ref_ptr<osg::Node> nodeDB = osgDB::readNodeFile("/Users/brandon/Programming/OpenSceneGraph-Data-2.8.0/glider.osg");
	osg::ref_ptr<osg::MatrixTransform> node = new osg::MatrixTransform();
	
	if (nodeDB != NULL)
		node->addChild(nodeDB.get());
	else
	{
		std::cout << "Problem with finding the model" << std::endl;
		return;
	}

	osgbBullet::MotionState *motion = new osgbBullet::MotionState;
	motion->setTransform(node.get());
	static btCollisionShape *cShape;
	if (cShape == NULL)
		cShape = osgbBullet::btConvexTriMeshCollisionShapeFromOSG(node.get());
//	osg::Node* debugNode = osgbBullet::osgNodeFromBtCollisionShape( cShape );
//    node->addChild( debugNode );
	
	btTransform shapeTransform;
	shapeTransform.setIdentity();
	shapeTransform.setOrigin(btVector3(0, 0, 0)); // change this to move the initial position of the object
	motion->setWorldTransform(shapeTransform);
	
	btVector3 inertia;
	cShape->calculateLocalInertia(_mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rbinfo(_mass, motion, cShape, inertia);
	btRigidBody *body = new btRigidBody(rbinfo);
	body->setLinearVelocity( btVector3( _aimingVector.x(), _aimingVector.y(), _aimingVector.z() ) );
	body->setAngularVelocity( btVector3( 1, 0, 0 ) );
	_dynamicsWorld->addRigidBody(body);
	
	_launchedObjects->addChild(node.get());
}

void BDScene::didChangeValueForKey(double value, aq::String key)
{
	if (key == "Update_Wand_Matrix")
	{
		osg::Vec3 direction(_wandMatrix.ptr()[8], _wandMatrix.ptr()[9],_wandMatrix.ptr()[10]);
		_navTrans->setMatrix(_navTrans->getMatrix() * osg::Matrix::translate(direction * value * 4.0));
	}
	else if (key == "Decrease_Nav_Speed")
	{
		if (_navSpeed > 0.12)
			_navSpeed -= 0.1;
	}
	else if (key == "Increase_Nav_Speed")
	{
		_navSpeed += 0.1;
	}
	else if (key == "Drop ball")
	{
		dropBall();
	}
	else if (key == "Aim_Left")
	{
		_aimingVector[0] -= 2;
	}
	else if (key == "Aim_Right")
	{
		_aimingVector[0] += 2;
	}
	else if (key == "Aim_Up")
	{
		_aimingVector[1] += 2;
	}
	else if (key == "Aim_Down")
	{
		_aimingVector[1] -= 2;
	}
	else if (key == "Mass_1")
	{
		_mass = 3.0;
		std::cout << "Object mass set to 3.0" << std::endl;
	}
	else if (key == "Mass_2")
	{
		_mass = 20.0;
		std::cout << "Object mass set to 20.0" << std::endl;
	}
	else if (key == "Mass_3")
	{
		_mass = 70.0;
		std::cout << "Object mass set to 70.0" << std::endl;
	}
	else if (key == "Reset_Scene")
	{
		std::cout << "Resetting scene" << std::endl;
		_resetScene();
	}
}

void BDScene::_resetScene()
{
	// Remove OSG objects
	_launchedObjects->removeChildren(0, _launchedObjects->getNumChildren());
	_boxes->removeChildren(0, _boxes->getNumChildren());
	
	// Remove Bullet objects by creating a new dynamics world
	delete _dynamicsWorld;
	initPhysics();
	
	// Add fresh stuff to the scene
	setupBoxes();
}

void BDScene::setHeadMatrix(osg::Matrixf mat)
{
	// Transform the head and wand matrix into navigation 
	// space (optional, depends on how you're using them)
	_headMatrix = mat * _navTrans->getInverseMatrix();
}

void BDScene::setWandMatrix(osg::Matrixf mat)
{
	_wandMatrix = mat * _navTrans->getInverseMatrix();
	_wandTrans->setMatrix(osg::Matrixf::scale(0.25, 0.125, 1.0) * _wandMatrix);
}

osg::Matrixf BDScene::getHeadMatrix()
{
	return _headMatrix;
}
osg::Matrixf BDScene::getWandMatrix()
{
	return _wandMatrix;
}

void BDScene::setNavigationMatrix(osg::Matrixf matrix)
{
	_navTrans->setMatrix(matrix);
}

osg::Matrixf BDScene::getNavigationMatrix()
{
	return _navTrans->getMatrix();
}

osg::Group* BDScene::getRootNode()
{
	return _rootNode.get();
}

osg::Group* BDScene::getModels()
{
	return _models.get();
}

void BDScene::buttonInput(int button, bool pressed)
{
	_deviceInputController->buttonInput(button, pressed);
}

void BDScene::update(double dt)
{
	// Update the total run-time
	_totalTime += dt;
	
	// Process all device inputs
	_deviceInputController->update(dt);
	
	// Update lighting
//	_lightsGroup->updateLights(_totalTime);
	
	// update physics
	_dynamicsWorld->stepSimulation(dt, 2);
}

osg::MatrixTransform* BDScene::createOSGBox( osg::Vec3 size )
{
    osg::Box * box = new osg::Box();
	
    box->setHalfLengths( size );
	
    osg::ShapeDrawable * shape = new osg::ShapeDrawable( box );
	
    osg::Geode * geode = new osg::Geode();
    geode->addDrawable( shape );
	
    osg::MatrixTransform * transform = new osg::MatrixTransform();
    transform->addChild( geode );
	
    return( transform );
}

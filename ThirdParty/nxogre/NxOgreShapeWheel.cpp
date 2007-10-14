//
//	NxOgre a wrapper for the PhysX (formerly Novodex) physics library and the Ogre 3D rendering engine.
//	Copyright (C) 2005 - 2007 Robin Southern and NxOgre.org http://www.nxogre.org
//
//	This library is free software; you can redistribute it and/or
//	modify it under the terms of the GNU Lesser General Public
//	License as published by the Free Software Foundation; either
//	version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//	Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public
//	License along with this library; if not, write to the Free Software
//	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "NxOgreStable.h"
#include "NxOgreShapeWheel.h"
#include "NxOgreShapeBlueprintWheel.h"	// For CubeShape
#include "NxOgreActor.h"
#include "NxOgreScene.h"
#include "NxOgreHelpers.h"
#include "NxOgreContainer.h"

namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////////////////

void WheelParams::setToDefault() {
	
	////////////

	motor_torque = 0;
	steer_angle = 0;
	brake_torque = 0.0f;
	mass = -1.0f;
	density = 1.0f;
	inverseWheelMass = 1.0f;

	///////////

	lateral_tire_asymptoteSlip = 2.0f;
	lateral_tire_asymptoteValue = 0.02f;
	lateral_tire_extremumSlip = 1.0f;
	lateral_tire_extremumValue = 0.02f;
	lateral_tire_stiffnessFactor = 1000000.0f;
	
	//////////

	longitudal_tire_asymptoteSlip = 2.0f;
	longitudal_tire_asymptoteValue = 0.02f;
	longitudal_tire_extremumSlip = 1.0f;
	longitudal_tire_extremumValue = 0.02f;
	longitudal_tire_stiffnessFactor = 1000000.0f;

	//////////

	suspension_damper = 0.0f;
	suspension_spring = 0.0f;
	suspension_target = 0.0f;
	suspension_travel = 1.0f;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void WheelParams::setToNormal() {
	
	////////////

	motor_torque = 0;
	steer_angle = 0;
	brake_torque = 0.0f;
	mass = 0;
	density = 1.0f;
	inverseWheelMass = 1.0f;

	///////////
	
	lateral_tire_asymptoteSlip = 2.0f;
	lateral_tire_asymptoteValue = 0.02f;
	lateral_tire_extremumSlip = 1.0f;
	lateral_tire_extremumValue = 0.02f;
	lateral_tire_stiffnessFactor = 1000000.0f;
	
	//////////

	longitudal_tire_asymptoteSlip = 2.0f;
	longitudal_tire_asymptoteValue = 0.02f;
	longitudal_tire_extremumSlip = 1.0f;
	longitudal_tire_extremumValue = 0.02f;
	longitudal_tire_stiffnessFactor = 1000000.0f;

	//////////

	suspension_damper = 50.0f;
	suspension_spring = 750.0f;
	suspension_target = 0.75f;
	suspension_travel = 1.0f;

}

////////////////////////////////////////////////////////////////////////////////////////////////

void WheelParams::parse(Parameters P) {

	setToDefault();

	for (Parameters::iterator p = P.begin(); p != P.end();p++) {
		if ((*p).first == "use-params" && ((*p).second.substr(0,1) == "N" || (*p).second.substr(0,1) == "n")) {
			setToNormal();
		}

		// and the rest
		//
		// if (Set("material", (*p), mMaterial)) continue;

	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

Wheel::Wheel(WheelShape& desc, WheelParams& params, Actor* actor) : Shape(actor) {
	desc.mShapeDescription.userData = this;

	
	desc.mShapeDescription.brakeTorque = params.brake_torque;
	//desc.mShapeDescription.ccdSkeleton;
	desc.mShapeDescription.density = params.density;
	//desc.mShapeDescription.group
	//desc.mShapeDescription.groupsMask;
	desc.mShapeDescription.inverseWheelMass = params.inverseWheelMass;
	desc.mShapeDescription.lateralTireForceFunction.asymptoteSlip = params.lateral_tire_asymptoteSlip;
	desc.mShapeDescription.lateralTireForceFunction.asymptoteValue = params.lateral_tire_asymptoteValue;
	desc.mShapeDescription.lateralTireForceFunction.extremumSlip = params.lateral_tire_extremumSlip;
	desc.mShapeDescription.lateralTireForceFunction.extremumValue = params.lateral_tire_extremumValue;
	desc.mShapeDescription.lateralTireForceFunction.stiffnessFactor = params.lateral_tire_stiffnessFactor;
	//desc.mShapeDescription.localPose;
	desc.mShapeDescription.longitudalTireForceFunction.asymptoteSlip = params.longitudal_tire_asymptoteSlip;
	desc.mShapeDescription.longitudalTireForceFunction.asymptoteValue = params.longitudal_tire_asymptoteValue;
	desc.mShapeDescription.longitudalTireForceFunction.extremumSlip = params.longitudal_tire_extremumSlip;
	desc.mShapeDescription.longitudalTireForceFunction.extremumValue = params.longitudal_tire_extremumValue;
	desc.mShapeDescription.longitudalTireForceFunction.stiffnessFactor = params.longitudal_tire_stiffnessFactor;


	desc.mShapeDescription.mass = params.mass;
//	desc.mShapeDescription.materialIndex;
	desc.mShapeDescription.motorTorque = params.motor_torque;
//	desc.mShapeDescription.name;
//	desc.mShapeDescription.radius

	// TODO:
	//desc.mShapeDescription.shapeFlags;
	
	//desc.mShapeDescription.skinWidth;
	desc.mShapeDescription.steerAngle = params.steer_angle.valueRadians();
	desc.mShapeDescription.suspension.damper = params.suspension_damper;
	desc.mShapeDescription.suspension.spring = params.suspension_spring;
	desc.mShapeDescription.suspension.targetValue = params.suspension_target;
	desc.mShapeDescription.suspensionTravel = params.suspension_travel;
	//desc.mShapeDescription.wheelContactModify = ..
	//desc.mShapeDescription.wheelFlags;

	/*
	// ------------ material  (default values)
	if( !mDummyMaterial )
	{
		NxMaterialDesc materialDesc;
		// It may be a good idea to use this when all friction is to be performed
		// using the tire friction model (see bellow)
		materialDesc.flags |=  NX_MF_DISABLE_FRICTION;
		mDummyMaterial = W->getNxWheelShape()->getActor().getScene().createMaterial(materialDesc);
	}
	W->getNxWheelShape()->setMaterial( mDummyMaterial->getMaterialIndex() );

	*/

	mNode = actor->getScene()->getSceneManager()->getRootSceneNode()->createChildSceneNode(NxCreateID(mActor->getNbShapes(), mActor->getName() + "wheel"));
	mWheelRollAngle = 0;
	mEntity = 0;
	mPose.zero();
}

////////////////////////////////////////////////////////////////////////////////////////////////

Wheel::~Wheel() {
	while (mNode->numAttachedObjects() > 0) {
		Ogre::Entity* ent = static_cast<Ogre::Entity*>( mNode->getAttachedObject(0) );
		mActor->getScene()->getSceneManager()->destroyEntity(ent);
	}

	mNode->detachAllObjects();
	mNode->removeAndDestroyAllChildren();
	mActor->getScene()->getSceneManager()->destroySceneNode(mNode->getName());

}

////////////////////////////////////////////////////////////////////////////////////////////////

void Wheel::_bindNxShapeToShape(NxShape* s) {
	mBaseShape = s;
	mWheel = static_cast<NxWheelShape*>(mBaseShape);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Wheel::simulate(float dt) {
	mWheelRollAngle += mWheel->getAxleSpeed() * dt;
	
	while (mWheelRollAngle > NxTwoPi)	//normally just 1x
		mWheelRollAngle -= NxTwoPi;
	while (mWheelRollAngle < -NxTwoPi)	//normally just 1x
		mWheelRollAngle += NxTwoPi;

    mPose = mWheel->getGlobalPose();


#if (NX_SDK_VERSION_MAJOR >= 2) && (NX_SDK_VERSION_MINOR >= 7)

    NxWheelContactData wcd;
	wcd.contactPosition = 0;

	NxShape* s = mWheel->getContact(wcd);

	NxReal  stravel = mWheel->getSuspensionTravel(), radius = mWheel->getRadius();

	if(s) {
		mPose.t += mWheel->getLocalOrientation() * mPose.M * NxVec3( 0.0, radius - wcd.contactPosition, 0.0);
	} else {
		mPose.t += mPose.M * NxVec3( 0.0, -stravel , 0.0 );
	}

	NxMat33 Heading, Pitch;
	Heading.rotY( mWheel->getSteerAngle() );
	Pitch.rotX( mWheelRollAngle );

	mPose.M = mPose.M * Heading * Pitch; 	

#else
   
	NxWheelContactData wcd;
    NxShape* s = mWheel->getContact(wcd);	
	NxReal  stravel = mWheel->getSuspensionTravel(), radius = mWheel->getRadius();

    //have ground contact?
	if( s && wcd.contactPosition <=  (stravel + radius) ) {
		mPose.t = NxVec3( mPose.t.x, wcd.contactPoint.y + radius, mPose.t.z );
    }
	else {
		mPose.t = NxVec3( mPose.t.x, mPose.t.y - stravel, mPose.t.z );
	}

    NxMat33 rot, axisRot, rollRot;
    rot.rotY( mWheel->getSteerAngle() );
    axisRot.rotY(0);
    rollRot.rotX(mWheelRollAngle);
    mPose.M = rot * mPose.M * axisRot * rollRot;

#endif

 	
	// user should take care of waking up, see Wheelset class
	// mWheel->getActor().wakeUp();

}

////////////////////////////////////////////////////////////////////////////////////////////////

void Wheel::render(float) {
	if (mNode) {
		mNode->setPosition( mPose.t.x, mPose.t.y, mPose.t.z );
		mNode->setOrientation( toQuaternion(mPose.M) );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Wheel::addMesh(const NxString& meshName) {
	if (mEntity == 0) {
		mEntity = mActor->getScene()->getSceneManager()->createEntity(NxCreateID(mActor->getNbShapes(), mNode->getName()), meshName);
		mNode->attachObject(mEntity);
	}
	else {
		Ogre::Entity* entity = mActor->getScene()->getSceneManager()->createEntity(NxCreateID(mActor->getNbShapes(), mNode->getName()), meshName);
		mNode->attachObject(entity);
	}
}

void Wheel::turn(Ogre::Radian angle) {
	mWheel->setSteerAngle(Ogre::Radian(mWheel->getSteerAngle()+angle.valueRadians()).valueRadians());
}
 
////////////////////////////////////////////////////////////////////////////

void Wheel::setRadius(float radius) {
	mWheel->setRadius(radius);
}
 
////////////////////////////////////////////////////////////////////////////

void Wheel::setSuspensionTravel (float travel) {
	mWheel->setSuspensionTravel(travel);
}
 
////////////////////////////////////////////////////////////////////////////

void Wheel::setMotorTorque(float torque) {
	mWheel->setMotorTorque(torque);
}
 
////////////////////////////////////////////////////////////////////////////

void Wheel::setBrakeTorque(float torque) {
	mWheel->setBrakeTorque(torque);
}
 
////////////////////////////////////////////////////////////////////////////

void Wheel::setSteeringAngle(Ogre::Radian angle) {
	mWheel->setSteerAngle(angle.valueRadians());
}
 
////////////////////////////////////////////////////////////////////////////

WheelSet WheelSet::createFourWheelSet(Actor* actor, Ogre::Vector3 frontLeft, Ogre::Vector3 backLeft, NxReal radius, ShapeParams sp, WheelParams wp) {

	WheelSet w;

	sp.mLocalPose.id();
	sp.mLocalPose.t = toNxVec3(frontLeft);

	w.frontLeft = static_cast<Wheel*>(actor->addShape(new WheelShape(radius, sp, wp)));
	
	frontLeft.x = -frontLeft.x;
	sp.mLocalPose.t = toNxVec3(frontLeft);
	w.frontRight = static_cast<Wheel*>(actor->addShape(new WheelShape(radius, sp, wp)));
	
	sp.mLocalPose.t = toNxVec3(backLeft);
	w.backLeft = static_cast<Wheel*>(actor->addShape(new WheelShape(radius, sp, wp)));
	
	backLeft.x = -backLeft.x;
	sp.mLocalPose.t = toNxVec3(backLeft);
	w.backRight = static_cast<Wheel*>(actor->addShape(new WheelShape(radius, sp, wp)));
	
	w.mWheels.insert(w.frontLeft);
	w.mWheels.insert(w.frontRight);
	w.mWheels.insert(w.backLeft);
	w.mWheels.insert(w.backRight);

	// TODO: FWD, RWD fixing.
	w.Drive.insert(w.frontLeft);
	w.Drive.insert(w.frontRight);
	w.Steering.insert(w.frontLeft);
	w.Steering.insert(w.frontRight);

	return w;
}
 
////////////////////////////////////////////////////////////////////////////

void WheelSet::addMeshes(Ogre::String meshName, Ogre::Vector3 offset) {
	for(Wheels::Iterator i = mWheels.items.begin();i != mWheels.items.end();++i) {
		(*i)->addMesh(meshName);
	}
}
 
////////////////////////////////////////////////////////////////////////////

void WheelSet::turn(Ogre::Radian angle) {
	
	for(Wheels::Iterator i = Steering.items.begin();i != Steering.items.end();++i) {
		(*i)->turn(angle);
	}
}
 
////////////////////////////////////////////////////////////////////////////

void WheelSet::setMotorTorque(NxReal torque) {
	for(Wheels::Iterator i = Drive.items.begin();i != Drive.items.end();++i) {
		(*i)->setMotorTorque(torque);
	}
}
 
////////////////////////////////////////////////////////////////////////////

void WheelSet::setBrakeTorque(NxReal torque) {
	for(Wheels::Iterator i = mWheels.items.begin();i != mWheels.items.end();++i) {
			(*i)->setBrakeTorque(torque);
	}
}
 
////////////////////////////////////////////////////////////////////////////

void WheelSet::attachDriveShaft(DriveShaft *ds) {
	mDriveShaft = ds;
}
 
////////////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.

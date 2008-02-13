#if 0
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
#include "NxOgreSceneRenderer.h"

#include "OgreSceneManager.h"
#include "OgreMovableObject.h"
#include "OgreEntity.h"

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

Wheel::Wheel(WheelShape* ws, WheelParams params, NodeRenderableParams visualParams, Actor* actor)
: Shape(actor), mWheelRollAngle(0) {

	ws->mShapeDescription.userData = this;
	ws->mShapeDescription.brakeTorque = params.brake_torque;
	ws->mShapeDescription.density = params.density;
	ws->mShapeDescription.inverseWheelMass = params.inverseWheelMass;
	ws->mShapeDescription.lateralTireForceFunction.asymptoteSlip = params.lateral_tire_asymptoteSlip;
	ws->mShapeDescription.lateralTireForceFunction.asymptoteValue = params.lateral_tire_asymptoteValue;
	ws->mShapeDescription.lateralTireForceFunction.extremumSlip = params.lateral_tire_extremumSlip;
	ws->mShapeDescription.lateralTireForceFunction.extremumValue = params.lateral_tire_extremumValue;
	ws->mShapeDescription.lateralTireForceFunction.stiffnessFactor = params.lateral_tire_stiffnessFactor;
	ws->mShapeDescription.longitudalTireForceFunction.asymptoteSlip = params.longitudal_tire_asymptoteSlip;
	ws->mShapeDescription.longitudalTireForceFunction.asymptoteValue = params.longitudal_tire_asymptoteValue;
	ws->mShapeDescription.longitudalTireForceFunction.extremumSlip = params.longitudal_tire_extremumSlip;
	ws->mShapeDescription.longitudalTireForceFunction.extremumValue = params.longitudal_tire_extremumValue;
	ws->mShapeDescription.longitudalTireForceFunction.stiffnessFactor = params.longitudal_tire_stiffnessFactor;
	ws->mShapeDescription.mass = params.mass;
	//ws->mShapeDescription.materialIndex;
	ws->mShapeDescription.motorTorque = params.motor_torque;
	//ws->mShapeDescription.name;
	//ws->mShapeDescription.radius

	// TODO:
	//ws->mShapeDescription.shapeFlags;
	//ws->mShapeDescription.skinWidth;
	ws->mShapeDescription.steerAngle = params.steer_angle.valueRadians();
	ws->mShapeDescription.suspension.damper = params.suspension_damper;
	ws->mShapeDescription.suspension.spring = params.suspension_spring;
	ws->mShapeDescription.suspension.targetValue = params.suspension_target;
	ws->mShapeDescription.suspensionTravel = params.suspension_travel;
	//ws->mShapeDescription.wheelContactModify = ..
	//ws->mShapeDescription.wheelFlags;

	/*
	// ------------ material  (default values)
	if( !mDummyMaterial )
	{
		NxMaterialDesc materialDesc;
		// It may be a good idea to use this when all friction is to be performed
		// using the tire friction model (see bellow)
		materialws.flags |=  NX_MF_DISABLE_FRICTION;
		mDummyMaterial = W->getNxWheelShape()->getActor().getScene().createMaterial(materialDesc);
	}
	W->getNxWheelShape()->setMaterial( mDummyMaterial->getMaterialIndex() );

	*/
	
	if (visualParams.Intent.size() == 0)
		visualParams.Intent = getStringType();

	setLevelOfDetail(LOD_Medium);
	setInterpolation(I_Linear);
	mRenderable = mActor->getScene()->getSceneRenderer()->createNodeRenderable(visualParams);
	mActor->getScene()->getSceneRenderer()->registerSource(this);

	mWheelLocalPose = ws->mShapeDescription.localPose;

	NxShape* s = actor->getNxActor()->createShape(mShapeDescription);

}

////////////////////////////////////////////////////////////////////////////////////////////////

Wheel::~Wheel() {

	mActor->getScene()->getSceneRenderer()->unregisterSource(this);

	if (mRenderable) {
		delete mRenderable;
		mRenderable = 0;
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

void Wheel::__renderSelf() {

	// (TODO) No interpolation, different thingys.

	switch (mLOD) {
		case LOD_High:
			{
				Pose calculated_pose = __calculatePositionHigh();
				Pose blended_pose = NxInterpolate(mLastPose, calculated_pose, mActor->getScene()->getLastAlphaValue());
				mLastPose = calculated_pose;
				mRenderable->setPose(blended_pose);
			}
			break; 
		case LOD_Medium:
			{
				Pose calculated_pose = __calculatePositionMedium();
				Pose blended_pose = NxInterpolate(mLastPose, calculated_pose, mActor->getScene()->getLastAlphaValue());
				mLastPose = calculated_pose;
				mRenderable->setPose(blended_pose);

			}
			break; 

		case LOD_Low: {
			{
				Pose calculated_pose = __calculatePositionLow();
				Pose blended_pose = NxInterpolate(mLastPose, calculated_pose, mActor->getScene()->getLastAlphaValue());
				mLastPose = calculated_pose;
				mRenderable->setPose(blended_pose);

			}
			break; 
		}
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

Pose Wheel::__calculatePositionHigh() {
	NxMat34 pose;	
	mWheelRollAngle += mWheel->getAxleSpeed() * mActor->getScene()->getLastDeltaTime();
	
	while (mWheelRollAngle > NxTwoPi)	//normally just 1x
		mWheelRollAngle -= NxTwoPi;
	while (mWheelRollAngle < -NxTwoPi)	//normally just 1x
		mWheelRollAngle += NxTwoPi;

	pose = mWheel->getGlobalPose();

//// 2.7.0
	NxWheelContactData wcd;
	wcd.contactPosition = 0;

	NxShape* contact_shape = mWheel->getContact(wcd);
	NxReal suspension_travel = mWheel->getSuspensionTravel();
	NxReal wheel_radius = mWheel->getRadius();

	if (contact_shape) {
		pose.t += mWheel->getLocalOrientation() * pose.M * NxVec3(0, wheel_radius - wcd.contactPosition, 0);
	}
	else {
		pose.t += pose.M * NxVec3(0, -suspension_travel, 0);
	}

	NxMat33 Heading, Pitch;
	Heading.rotY(mWheel->getSteerAngle());
	Pitch.rotX(mWheelRollAngle);

	pose.M = pose.M * Heading * Pitch;

////////////

	Pose return_pose;
	return_pose.v = pose.t;
	pose.M.toQuat(return_pose.q);

	return return_pose;

}

////////////////////////////////////////////////////////////////////////////////////////////////

Pose Wheel::__calculatePositionMedium() {
	
	mWheelRollAngle += mWheel->getAxleSpeed() * mActor->getScene()->getLastDeltaTime();
	
	while (mWheelRollAngle > NxTwoPi)	//normally just 1x
		mWheelRollAngle -= NxTwoPi;
	while (mWheelRollAngle < -NxTwoPi)	//normally just 1x
		mWheelRollAngle += NxTwoPi;


	NxMat34 pose;
	pose = mWheel->getGlobalPose();

	NxWheelContactData wcd;
	NxShape* s = mWheel->getContact(wcd);	

	NxReal r = mWheel->getRadius();
	NxReal st = mWheel->getSuspensionTravel();
	NxReal steerAngle = mWheel->getSteerAngle();

	//					NxWheelShapeDesc state;	
	//					mWheel->saveToDesc(state);

	NxVec3 p0;
	NxVec3 dir;
	/*
	getWorldSegmentFast(seg);
	seg.computeDirection(dir);
	dir.normalize();
	*/
	p0 = pose.t;  //cast from shape origin
	pose.M.getColumn(1, dir);
	dir = -dir;	//cast along -Y.
	NxReal castLength = r + st;	//cast ray this long

	//					renderer.addArrow(p0, dir, castLength, 1.0f);

	//have ground contact?
	// This code is from WheelShape.cpp in SDKs/core/common/src
	// if (contactPosition != NX_MAX_REAL)  
	if (s && wcd.contactForce > -1000)
	{
	//						pose.t = p0 + dir * wcd.contactPoint;
	//						pose.t -= dir * state.radius;	//go from contact pos to center pos.
		pose.t = wcd.contactPoint;
		pose.t -= dir * r;	//go from contact pos to center pos.
	}
	else
	{
		pose.t = p0 + dir * st;
	}

	NxMat33 rot, axisRot;
	rot.rotY(steerAngle);
	axisRot.rotY(0);

	//					NxReal rollAngle = ((ShapeUserData*)(wheel->userData))->rollAngle;

	NxMat33 rollRot;
	rollRot.rotX(mWheelRollAngle);

	pose.M = rot * pose.M * axisRot * rollRot;


	Pose return_pose;
	return_pose.v = pose.t;
	pose.M.toQuat(return_pose.q);
	return return_pose;

}

////////////////////////////////////////////////////////////////////////////////////////////////

Pose Wheel::__calculatePositionLow() {
	
	NxMat34 actor_pose = mActor->getNxActor()->getGlobalPose();
	
	NxMat34 pose = actor_pose * mWheelLocalPose;

	mWheelRollAngle += mWheel->getAxleSpeed() * mActor->getScene()->getLastDeltaTime();
	
	while (mWheelRollAngle > NxTwoPi)	//normally just 1x
		mWheelRollAngle -= NxTwoPi;
	while (mWheelRollAngle < -NxTwoPi)	//normally just 1x
		mWheelRollAngle += NxTwoPi;

	NxMat33 Heading, Pitch;
	Heading.rotY(mWheel->getSteerAngle());
	Pitch.rotX(mWheelRollAngle);
	pose.M = pose.M * Heading * Pitch;

	Pose return_pose;
	return_pose.v = pose.t;
	pose.M.toQuat(return_pose.q);
	return return_pose;
}

////////////////////////////////////////////////////////////////////////////////////////////////

#if 0
{
	mWheelRollAngle += mWheel->getAxleSpeed() * dt;
	
	while (mWheelRollAngle > NxTwoPi)	//normally just 1x
		mWheelRollAngle -= NxTwoPi;
	while (mWheelRollAngle < -NxTwoPi)	//normally just 1x
		mWheelRollAngle += NxTwoPi;

    mPose = mWheel->getGlobalPose();


#if (NX_SDK_VERSION_NUMBER >= 270)

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
#endif
////////////////////////////////////////////////////////////////////////////

void Wheel::turn(Ogre::Radian angle) {
	mWheel->setSteerAngle(Ogre::Radian(mWheel->getSteerAngle()+angle.valueRadians()).valueRadians());
}
 
////////////////////////////////////////////////////////////////////////////

void Wheel::setRadius(float radius) {
	mWheel->setRadius(radius);
}
 
////////////////////////////////////////////////////////////////////////////

void Wheel::setSuspensionTravel(float travel) {
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
 

#if 0
////////////////////////////////////////////////////////////////////////////

WheelSet WheelSet::createFourWheelSet(Actor* actor, Ogre::Vector3 frontLeft, Ogre::Vector3 backLeft, NxReal radius, ShapeParams sp, WheelParams wp) {

	WheelSet w;

	sp.mLocalPose.id();
	sp.mLocalPose.t = NxConvert<NxVec3, Ogre::Vector3>(frontLeft);

	w.frontLeft = static_cast<Wheel*>(actor->addShape(new WheelShape(radius, sp, wp)));
	
	frontLeft.x = -frontLeft.x;
	sp.mLocalPose.t = NxConvert<NxVec3, Ogre::Vector3>(frontLeft);
	w.frontRight = static_cast<Wheel*>(actor->addShape(new WheelShape(radius, sp, wp)));
	
	sp.mLocalPose.t = NxConvert<NxVec3, Ogre::Vector3>(backLeft);
	w.backLeft = static_cast<Wheel*>(actor->addShape(new WheelShape(radius, sp, wp)));
	
	backLeft.x = -backLeft.x;
	sp.mLocalPose.t = NxConvert<NxVec3, Ogre::Vector3>(backLeft);
	w.backRight = static_cast<Wheel*>(actor->addShape(new WheelShape(radius, sp, wp)));
	
	w.mWheels.insert(0, w.frontLeft);
	w.mWheels.insert(1, w.frontRight);
	w.mWheels.insert(2, w.backLeft);
	w.mWheels.insert(3, w.backRight);

	std::cout << "Number of Wheels => " << mWheels.count() << std::endl;
	
	// TODO: FWD, RWD fixing.
	w.Drive.insert(0, w.frontLeft);
	w.Drive.insert(1, w.frontRight);
	w.Steering.insert(0, w.frontLeft);
	w.Steering.insert(1, w.frontRight);

	return w;
}
 
////////////////////////////////////////////////////////////////////////////

void WheelSet::addMeshes(Ogre::String meshName, Ogre::Vector3 offset) {
	std::cout << "Adding wheels (" << mWheels.count() << ")" << std::endl;
	for(Wheel* wheel = mWheels.begin();wheel = mWheels.next();) {
		wheel->addMesh(meshName);
		std::cout << " -> " << wheel << std::endl;
	}
}
 
////////////////////////////////////////////////////////////////////////////

void WheelSet::turn(Ogre::Radian angle) {
	
	for(Wheel* wheel = mWheels.begin();wheel = mWheels.next();) {
		wheel->turn(angle);
	}
}
 
////////////////////////////////////////////////////////////////////////////

void WheelSet::setMotorTorque(NxReal torque) {
	for(Wheel* wheel = mWheels.begin();wheel = mWheels.next();) {
		wheel->setMotorTorque(torque);
	}
}
 
////////////////////////////////////////////////////////////////////////////

void WheelSet::setBrakeTorque(NxReal torque) {
	for(Wheel* wheel = mWheels.begin();wheel = mWheels.next();) {
		wheel->setBrakeTorque(torque);
	}
}
 
////////////////////////////////////////////////////////////////////////////

void WheelSet::attachDriveShaft(DriveShaft *ds) {
	mDriveShaft = ds;
}
 
////////////////////////////////////////////////////////////////////////////

void Wheel::setEntity(Ogre::Entity* e) {
	
	mNode->attachObject(e);
	
	if (mEntity==0) {
		mEntity = e;
	}

}

#endif 

}; //End of NxOgre namespace.
#endif

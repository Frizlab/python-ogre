#include ".\conveyormatcallback.h"
#include ".\conveyorBelt.h"
import ogre.renderer.OGRE as Ogre
import ogre.physics.ogrenewt2 as OgreNewt

class conveyorMatCallback ( OgreNewt.ContactCallback ):

    def __init__ ( self, conveyorID ) :
        OgreNewt.ContactCallback.__init__(self) 
        self.ConveyorID = conveyorID
    
            
    def contactsProcess( self, contactJoint, timeStep,  threadIndex ):
        body0 = contactJoint.getBody0()
        body1 = contactJoint.getBody1()
        belt = None
        
        if (body0.getType() == self.ConveyorID):
            belt = body0.getUserData().getData()[1] # we know it's a python object
            object = body1
        if (body1.getType() == self.ConveyorID):
            belt = body1.getUserData().getData()[1]
            object = body0
        if not belt:
            return
        ## okay, found the belt... let's adjust the collision based on this.
        thedir = belt.getGlobalDir()
        count = contactJoint.getContactCount()
        contact = contactJoint.getFirstContact()
        for x in range (count):
            contact.rotateTangentDirections( thedir )
            contactPos = Ogre.Vector3()
            contactNorm = Ogre.Vector3()
            contact.getPositionAndNormal(contactPos, contactNorm)
            objectPos = Ogre.Vector3()
            objectOri = Ogre.Quaternion()
            object.getPositionOrientation(objectPos, objectOri)
            objectContactPointVel = object.getVelocity() + (contactPos - objectPos)*object.getOmega()
            result_accel = belt.getSpeed() - thedir.dotProduct(objectContactPointVel)
            result_accel *= 10; # looks nicer
            contact.setTangentAcceleration( result_accel, 0 )
            if x < (count - 1): # don't call getNext on last pass as causes crash
                contact = contact.getNext()
        return
        
    def onAABBOverlap( self,  body0, body1, threadIndex ):
        if( body0.getType() == self.ConveyorID or body1.getType() == self.ConveyorID ):
            return 1
        return 0
        

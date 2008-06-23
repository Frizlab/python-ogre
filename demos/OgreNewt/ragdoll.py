#include ".\Ragdoll.h"
import ogre.renderer.OGRE as Ogre
import ogre.physics.OgreNewt as OgreNewt
import ctypes
import sys
from xml.dom.minidom import parse, parseString

class RagDoll:
    JT_BALLSOCKET = 0
    JT_HINGE = 1

    class RagBone:
        BS_BOX =1
        BS_ELLIPSOID=2
        BS_CYLINDER=3
        BS_CAPSULE=4
        BS_CONE=5
        BS_CONVEXHULL=6

        def __init__( self, creator,world, parent, ogreBone, mesh, 
                          dir1, shape, size, mass ):

            self.Doll = creator 
            self.Parent = parent 
            self.OgreBone = ogreBone 
        
            ## in the case of the cylindrical primitives, they need to be rotated to align the main axis with the direction vector.
            orient = Ogre.Quaternion.IDENTITY 
            pos = Ogre.Vector3.ZERO 
            rot = Ogre.Matrix3()
        
            if (dir1 == Ogre.Vector3.UNIT_Y):
                rot.FromEulerAnglesXYZ(Ogre.Degree(0), Ogre.Degree(0), Ogre.Degree(90)) 
                orient.FromRotationMatrix( rot ) 
        
            if (dir1 == Ogre.Vector3.UNIT_Z):
                rot.FromEulerAnglesXYZ(Ogre.Degree(0), Ogre.Degree(90), Ogre.Degree(0)) 
                orient.FromRotationMatrix( rot ) 

            ## make the rigid body.
            if shape == RagDoll.RagBone.BS_BOX:
                col = OgreNewt.Box( world, size ) 
            elif shape == RagDoll.RagBone.BS_CAPSULE:
                col = OgreNewt.Capsule( world, size.y, size.x, orient, pos ) 
            elif shape == RagDoll.RagBone.BS_CONE:
                col = OgreNewt.Cone( world, size.y, size.x, orient, pos ) 
            elif shape == RagDoll.RagBone.BS_CYLINDER:
                col = OgreNewt.Cylinder( world, size.y, size.x, orient, pos ) 
            elif shape == RagDoll.RagBone.BS_ELLIPSOID:
                col = OgreNewt.Ellipsoid( world, size ) 
            elif shape == RagDoll.RagBone.BS_CONVEXHULL:
                col = self._makeConvexHull( world, mesh, size.x ) 
            else:
                col = OgreNewt.Box( world, size ) 
        
            self.Body = OgreNewt.Body( world, col ) 
            self.Body.setUserData( self ) 
            self.Body.setStandardForceCallback() 
        
            inertia, com = col.calculateInertialMatrix( ) 
            
            self.Body.setMassMatrix( mass, inertia * mass ) 
            self.Body.setCenterOfMass( com ) 
        
            self.Body.setCustomTransformCallback( self.Doll, "_placementCallback" ) 


        def __del__(self): ## ragbone
            self.OgreBone.setManuallyControlled( False ) 
            del self.Body 
       


        def _makeConvexHull( self, world, mesh, minWeight ):
            ## for self bone, gather all of the vertices linked to it, and make an individual convex hull.
            boneName = self.OgreBone.getName() 
            boneIndex = self.OgreBone.getHandle() 
            invMatrix  = Ogre.Matrix4 ()
            invMatrix.makeInverseTransform( -self.OgreBone._getBindingPoseInversePosition(), 
                                Ogre.Vector3.UNIT_SCALE / self.OgreBone._getBindingPoseInverseScale(), 
                                self.OgreBone._getBindingPoseInverseOrientation().Inverse()) 
        
            num_sub = mesh.getNumSubMeshes() 
        
            for i in range (num_sub):
                submesh = mesh.getSubMesh(i) 
                bai = submesh.getBoneAssignmentIterator() 
        
                if (submesh.useSharedVertices):
                    v_data = mesh.sharedVertexData 
                    v_count = v_data.vertexCount 
                    v_decl = v_data.vertexDeclaration 
                    p_elem = v_decl.findElementBySemantic( Ogre.VES_POSITION ) 
                else:
                    v_data = submesh.vertexData 
                    v_count = v_data.vertexCount 
                    v_decl = v_data.vertexDeclaration 
                    p_elem = v_decl.findElementBySemantic( Ogre.VES_POSITION ) 
                    
                start = v_data.vertexStart 
                
                ## Here's were we start messing with 'C' pointers
                v_sptr = v_data.vertexBufferBinding.getBuffer( p_elem.getSource() ) #returns a hardwarevertexbuffer
                v_ptr = v_sptr.lock( Ogre.HardwareBuffer.HBL_READ_ONLY )    # returns a void pointer
                
                v_ptr_address = Ogre.CastInt ( v_ptr )  ##need to write this one...
                
                ## we need to reference floats in groups of 3...
                _storageclass = ctypes.c_float * (3)
                vertexVector=[]
                while (bai.hasMoreElements()):
                    vba = bai.getNext() 
                    if (vba.boneIndex == boneIndex):
                        ##found a vertex that is attached to self bone.
                        if (vba.weight >= minWeight):
                            
                            ## We want the the offset into the vertex buffer.
                            v_offset = v_ptr_address + (vba.vertexIndex * v_sptr.getVertexSize()) 
                            
                            ## And now we get a pointer to the actual buffer position..
                            ## 
                            ## C++ uses this function as it returns a different pointer based upon the 
                            ## second argumment (v_Posptr)..  
                            ## In python we don't need this, instead we simply want the address so we can
                            ## use ctypes to cope with the differnt underlying 'C' type
                            ##
                            ## p_elem.baseVertexPointerToElement( v_offset, v_Posptr ) 
                            
                            v_offset = v_offset + p_elem.getOffset()
                            
                            _FloatBuffer = _storageclass.from_address( v_offset )
                            
                            vert = Ogre.Vector3()
                            vert.x = _FloatBuffer[0]
                            vert.y = _FloatBuffer[1]
                            vert.z = _FloatBuffer[2]  
                            
                            ## apply transformation in to local space.
                            vert = invMatrix * vert 
                            ## and save the Vector3...
                            vertexVector.append( vert ) 
                v_sptr.unlock() 
                
       
            ##################################################################################
#             col = OgreNewt.CollisionPrimitives.ConvexHull( world, verts, numVerts ) 
            ## To Check - might need to keep vertexVector around - ie as a self.vertexVector
            ## Note the helper function -- the Constructor on ConvexHull needs a pointer to a list of Vector3's
            col = OgreNewt.createConvexHull( world, vertexVector, len(vertexVector) )
                            #.Ogre.Quaternion.IDENTITY, Ogre.Vector3.ZERO ) 
            return col  
            
        ## newton body.
        def getBody(self):
            return self.Body

        ## set limits.
        def setLimits( self, limit1, limit2 ):
            self.Limit1 = limit1
            self.Limit2 = limit2

        ## get limits.
        def getLimit1(self):
            return self.Limit1
            
        def getLimit2(self):
            return self.Limit2

        ## get ogre bone
        def getOgreBone(self):
            return self.OgreBone

        ## get parent.
        def getParent( self ):
            return self.Parent

        ## get doll.
        def getDoll( self ):
            return self.Doll

        ## set offset.
        def setOffset( self, orient, pos ):
            self.OffsetOrient = orient
            self.OffsetPos = pos

        ## get offsets
        def getOffsetOrient( self ):
            return self.OffsetOrient
        def getOffsetPos( self ):
            return self.OffsetPos
        
        
        
        

    def __init__( self, filename, world, node ):    # ragdoll __init__
        self.Node = node 
        self.World = world 
        self.Bones=[]
        self.statics=[]
        ## get the skeleton.
        self.Skeleton = self.Node.getAttachedObject(0).getSkeleton() 
        ## get the mesh.
        self.Mesh = self.Node.getAttachedObject(0).getMesh() 

        ## now for XML handling        
        doc = parse(filename) # parse an XML file by name
        root = doc.documentElement
        if (not root):
            Ogre.LogManager.getSingleton().logMessage(" ERROR! cannot find 'root' in xml file: "+filename ) 
            return 
        ## found the root ragdoll.  find the root bone, and go!
        bone = root.getElementsByTagName ("Bone")[0]

        ##bone = root.FirstChildElement("Bone") 
        if (bone):
            self._addAllBones( None, bone ) 
        else:
            sys.exit("Issue - master bone is missing")

    def _addAllBones ( self, parent,  bone):
        ## get the information for the bone represented by self element.
        t =  bone.getAttribute("dir").split()  #vector3
        dire = Ogre.Vector3(float(t[0]), float(t[1]), float(t[2]))
        
        length =  float(bone.getAttribute("length") )# real
            
        t =  bone.getAttribute("size").split() #vector3
        size = Ogre.Vector3(float(t[0]), float(t[1]), float(t[2]))
        
        skeleton_bone = bone.getAttribute("skeleton_bone") 
        ogrebone = self.Skeleton.getBone( str(skeleton_bone) ) 
    
        shapestr = bone.getAttribute("shape") 
        shape = RagDoll.RagBone.BS_BOX 
    
        if (shapestr=="box"):
            shape = RagDoll.RagBone.BS_BOX 
        elif (shapestr=="capsule"):
            shape = RagDoll.RagBone.BS_CAPSULE 
        elif (shapestr=="cylinder"):
            shape = RagDoll.RagBone.BS_CYLINDER 
        elif (shapestr=="cone"):
            shape = RagDoll.RagBone.BS_CONE 
        elif (shapestr=="ellipsoid"):
            shape = RagDoll.RagBone.BS_ELLIPSOID 
        elif (shapestr=="hull"):
            shape = RagDoll.RagBone.BS_CONVEXHULL 
    
        mass = float( bone.getAttribute("mass") ) 
        
        ##############################################################################/
        me = self._addBone( self.World, parent, dire, shape, size, mass, ogrebone ) 
        ##############################################################################/
    
        ## position the bone.
        boneorient = self.Node.getOrientation() * ogrebone._getDerivedOrientation() 
        if (shape != RagDoll.RagBone.BS_CONVEXHULL):
            bonepos = self.Node._getFullTransform() * ogrebone._getDerivedPosition() + (boneorient * (dire * (length*0.5))) 
        else:
            bonepos = self.Node._getFullTransform() * ogrebone._getDerivedPosition()
        me.Body.setPositionOrientation( bonepos, boneorient ) 
    
        ## set offsets
        if (not parent):
            offsetorient = (boneorient.Inverse()) * self.Node.getOrientation() 
            offsetpos = boneorient.Inverse() * (self.Node.getPosition() - bonepos) 
            me.setOffset( offsetorient, offsetpos ) 
    
        ## get the joint to connect self bone with it's parent.
        if (parent):
            joint = bone.getElementsByTagName ("Joint")[0]
            if (not joint):
                ## error!
                Ogre.LogManager.getSingleton().logMessage(" ERROR! cannot find 'Joint' in xml file!") 
                return 
        
            p = joint.getAttribute("pin").split()
            jointpin = Ogre.Vector3(float(p[0]), float(p[1]), float(p[2]))
        
 
            jointtypestr = joint.getAttribute("type") 
            jointtype = RagDoll.JT_BALLSOCKET 
    
            if (jointtypestr == "ballsocket"):
                jointtype = RagDoll.JT_BALLSOCKET 
            elif (jointtypestr == "hinge"):
                jointtype = RagDoll.JT_HINGE 
    
            limit1 = float( joint.getAttribute("limit1") ) 
            limit2 = float( joint.getAttribute("limit2") ) 
    
            jpos = self.Node._getFullTransform() * ogrebone._getDerivedPosition() 
            jpin = (self.Node.getOrientation() * parent.getOgreBone()._getDerivedOrientation()) * jointpin 
    
            self._joinBones( jointtype, parent, me, jpos, jpin, limit1, limit2) 
    
        ##############################################################################/
        ##############################################################################/
        ## add all children of self bone.
        for b in  bone.childNodes:
            if b.nodeName == "Bone":
               self._addAllBones( me, b )
        
    
    
    #RagDoll.~RagDoll()
    def __del__(self) :
        while (self.Bones.size() > 0):
            bone = self.Bones.back() 
            del bone 
            self.Bones.pop() 

    #RagDoll.RagBone* RagDoll
    def _addBone( self, world, parent, dire, shape, size, mass, ogrebone ):
        bone = RagDoll.RagBone( self, world, parent, ogrebone, self.Mesh, dire, shape, size, mass ) 
        ogrebone.setManuallyControlled( True ) 
        self.Bones.append( bone ) 
        return bone 
    
    
    def _joinBones( self, typein, parent, child, pos, pin, limit1, limit2 ):
        pin.normalise() 
        if typein == RagDoll.JT_BALLSOCKET:
            joint = OgreNewt.BallAndSocket( child.Body.getWorld(), child.Body, parent.Body, pos ) 
            joint.setLimits(pin, Ogre.Degree(limit1), Ogre.Degree(limit2)) 
            self.statics.append(joint) # need to keep it arround 
        elif typein == RagDoll.JT_HINGE:
            joint = OgreNewt.Hinge( child.Body.getWorld(), child.Body, parent.Body, pos, pin ) 
            joint.setCallback( _hingeCallback,"" ) 
            joint.setUserData( child ) 
            child.setLimits( limit1, limit2 )
            self.statics.append(joint) # need to keep it arround 
    
    
    def _placementCallback(  self, me,  orient,  pos ):
        bone = me.getUserData() 
        doll = bone.getDoll() 
    
        ## is self the root bone?
        if (not bone.getParent()):
            finalorient = (orient * bone.getOffsetOrient()) 
            finalpos = pos + (orient * bone.getOffsetPos()) 
            doll.Node.setPosition( finalpos ) 
            doll.Node.setOrientation( finalorient ) 
        else:
            ## standard bone, calculate the local orientation between it and it's parent.
            parentpos, parentorient = bone.getParent().Body.getPositionOrientation(  ) 
            localorient = parentorient.Inverse() * orient 
            bone.getOgreBone().setOrientation( localorient ) 
            

    def inheritVelOmega( self, vel, omega ):
        ## find main position.
        mainpos = self.Node.getWorldPosition() 
        it = self.Bones.begin()
        while it:
        ## for (RagBoneListIterator it = self.Bones.begin()  it != self.Bones.end(); it++)
            pos, orient = it.Body.getPositionOrientation(  ) 
            it.Body.setVelocity( vel + omega.crossProduct( pos - mainpos ) ) 
    
def _hingeCallback( me ):
    bone = me.getUserData() 
    angle = me.getJointAngle() 
    lim1 = bone.getLimit1() 
    lim2 = bone.getLimit2() 
    if (angle < lim1):
        accel = me.calculateStopAlpha( lim1 ) 
        me.setCallbackAccel( accel ) 
    if (angle > lim2):
        accel = me.calculateStopAlpha( lim2 ) 
        me.setCallbackAccel( accel ) 
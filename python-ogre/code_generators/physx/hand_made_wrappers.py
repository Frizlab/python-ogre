import os
import environment

WRAPPER_WRAPPER_DEF_NxForceFieldShapeGroup = \
"""
    virtual void releaseShape( ::NxForceFieldShape const & arg0 ){
        bp::override func_releaseShape = this->get_override( "releaseShape" );
        func_releaseShape( boost::ref(arg0) );
    }
"""
    
WRAPPER_DEFINITION_NxScene = \
"""bp::tuple NxScene_getTiming( ::NxScene & me) { 
    NxReal maxTimestep;
	NxTimeStepMethod method;
	NxU32 maxIter;
	NxU32 numSubSteps;
	me.getTiming(maxTimestep, maxIter, method, &numSubSteps);
	return ( boost::python::make_tuple( maxTimestep, maxIter, method, numSubSteps ) );
	}
"""	
WRAPPER_WRAPPER_DEF_NxScene = \
"""
    virtual void getTiming(NxReal& maxTimestep, NxU32& maxIter, NxTimeStepMethod& method, NxU32* numSubSteps=NULL) const {
    // placeholder so wrapper will compile
    }
"""    

WRAPPER_REGISTRATION_NxScene = [
    'def( "getTiming", &::NxScene_getTiming,\
    "Python-Ogre Hand Wrapped\\nReturns a tuple with maxTimestep, maxIter, method, numSubSteps");'
#     ,\
#     bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());', 
    ]

WRAPPER_DEFINITION_NxStream = \
""" NxStream&	NxStream_storeBuffer(NxStream & me, const void* buffer, NxU32 size)	 { 
// NOT YET IMPLEMENTED
    NxStream result;
    return result;
}
"""
  
WRAPPER_WRAPPER_DEF_NxStream = \
"""
    virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size)	 {
    // placeholder so wrapper will compile
    }
""" 
       
WRAPPER_REGISTRATION_NxStream  = [
    'def( "storeBuffer", &::NxStream_storeBuffer,\
    "Python-Ogre Hand Wrapped NOT IMPLEMENTED\\nReturns a NxStream -- takes a python list",\
    bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());', 
    ]
    
        		
WRAPPER_DEFINITION_NxCompartment = \
"""bp::tuple NxCompartment_getTiming( ::NxCompartment & me) { 
    NxReal maxTimestep;
	NxTimeStepMethod method;
	NxU32 maxIter;
	NxU32 numSubSteps;
	me.getTiming(maxTimestep, maxIter, method, &numSubSteps);
	return ( boost::python::make_tuple( maxTimestep, maxIter, method, numSubSteps ) );
	}
"""	

WRAPPER_REGISTRATION_NxCompartment = [
    'def( "getTiming", &::NxCompartment_getTiming,\
    "Python-Ogre Hand Wrapped\\nReturns a tuple with maxTimestep, maxIter, method, numSubSteps");',
    ]
    
WRAPPER_DEFINITION_NxConvexMesh= \
"""bp::tuple NxConvexMesh_getMassInformation( ::NxConvexMesh & me) { 
    NxReal mass;
    NxMat33 localInertia;
    NxVec3 localCenterOfMass;
    me.getMassInformation( mass, localInertia, localCenterOfMass );
	return ( boost::python::make_tuple( mass, localInertia, localCenterOfMass ) );
	}
"""	
WRAPPER_REGISTRATION_NxConvexMesh = [
    'def( "getMassInformation", &::NxConvexMesh_getMassInformation,\
    "Python-Ogre Hand Wrapped\\nReturns a tuple with mass, localInertia, localCenterOfMass");',
    ]

WRAPPER_DEFINITION_NxTriangleMesh= \
"""bp::tuple NxTriangleMesh_getMassInformation( ::NxTriangleMesh & me) { 
    NxReal mass;
    NxMat33 localInertia;
    NxVec3 localCenterOfMass;
    me.getMassInformation( mass, localInertia, localCenterOfMass );
	return ( boost::python::make_tuple( mass, localInertia, localCenterOfMass ) );
	}
"""	
WRAPPER_REGISTRATION_NxTriangleMesh = [
    'def( "getMassInformation", &::NxTriangleMesh_getMassInformation,\
    "Python-Ogre Hand Wrapped\\nReturns a tuple with mass, localInertia, localCenterOfMass");',
    ]
    
WRAPPER_WRAPPER_DEF_NxPhysicsSDK= \
"""
   virtual ::NxU32 getInternalVersion( ::NxU32 & apiRev, ::NxU32 & descRev, ::NxU32 & branchId ) const {
    // dummy function as place holder
        return 0;
    };
"""    
    
WRAPPER_DEFINITION_NxPhysicsSDK= \
"""bp::tuple NxPhysicsSDK_getInternalVersion(::NxPhysicsSDK & me ) { 
    NxU32  apiRev;
    NxU32  descRev;
    NxU32  branchId;
    NxU32  sdkRev;
    sdkRev = me.getInternalVersion( apiRev, descRev, branchId );
	return ( boost::python::make_tuple( sdkRev, apiRev, descRev, branchId ) );
	}
"""	    
WRAPPER_REGISTRATION_NxPhysicsSDK = [
    'def( "getInternalVersion", &::NxPhysicsSDK_getInternalVersion,\
    "Python-Ogre Hand Wrapped\\nReturns a tuple with sdkRev, apiRev, descRev, branchId");',
    ]
    
WRAPPER_DEFINITION_NxActor_Wrapper = \
"""
virtual ::NxShape * const * getShapes(  ) const {
    bp::override func_getShapes = this->get_override( "getShapes" );
    return func_getShapes( );
    }
"""

WRAPPER_DEFINITION_Utility = \
"""
unsigned int NxUtility_NX_PHYSICS_SDK_VERSION( void ) {
    return NX_PHYSICS_SDK_VERSION; 
    }
    
boost::python::tuple 
NxUtility_NX_PHYSICS_SDK_VERSION_TUPLE ( void ) {
            return ( boost::python::make_tuple( NX_PHYSICS_SDK_VERSION,
                                                NX_SDK_VERSION_MAJOR,
                                                NX_SDK_VERSION_MINOR,
                                                NX_SDK_VERSION_BUGFIX
                                                ) );
}   

boost::python::tuple NxUtility_NxCreatePhysicsSDK( ::NxU32 sdkVersion, ::NxUserAllocator * allocator=0, ::NxUserOutputStream * outputStream=0, ::NxPhysicsSDKDesc const & desc=::NxPhysicsSDKDesc( ) ){
    NxSDKCreateError errorCode2;
    ::NxPhysicsSDK * result = ::NxCreatePhysicsSDK(sdkVersion, allocator, outputStream, desc, &errorCode2);
    typedef bp::return_value_policy< bp::reference_existing_object > call_policies_t;
    return bp::make_tuple( pyplusplus::call_policies::make_object< call_policies_t, ::NxPhysicsSDK * >( result )
                            , errorCode2 );
}

boost::python::tuple NxUtility_NxCreatePhysicsSDKWithID( ::NxU32 sdkVersion, char * companyNameStr, char * appNameStr, char * appVersionStr, char * appUserDefinedStr, ::NxUserAllocator * allocator=0, ::NxUserOutputStream * outputStream=0, ::NxPhysicsSDKDesc const & desc=::NxPhysicsSDKDesc( ) ){
    NxSDKCreateError errorCode2;
    ::NxPhysicsSDK * result = ::NxCreatePhysicsSDKWithID(sdkVersion, companyNameStr, appNameStr, appVersionStr, appUserDefinedStr, allocator, outputStream, desc, &errorCode2);
    typedef bp::return_value_policy< bp::reference_existing_object > call_policies_t;
    return bp::make_tuple( pyplusplus::call_policies::make_object< call_policies_t, ::NxPhysicsSDK * >( result )
                            , errorCode2 );
}

 
"""
WRAPPER_REGISTRATION_Utility = [
    'bp::def( "NX_PHYSICS_SDK_VERSION", &::NxUtility_NX_PHYSICS_SDK_VERSION,\
    "Python-Ogre Hand Wrapped\\nReturns the SDK Version");',
    
    'bp::def( "NX_PHYSICS_SDK_VERSION_TUPLE", &::NxUtility_NX_PHYSICS_SDK_VERSION_TUPLE,\
    "Python-Ogre Hand Wrapped\\nReturns the SDK Version, A tuple of version,major,minor,bugfix");',
    
    """bp::def( "NxCreatePhysicsSDK", &::NxUtility_NxCreatePhysicsSDK,
    ( bp::arg("sdkVersion"), bp::arg("allocator")=bp::object(), bp::arg("outputStream")=bp::object(), bp::arg("desc")=::NxPhysicsSDKDesc( ) ),
    "Python-Ogre Hand Wrapped (PhysX) Modified Function Call\\n\\
    Input: \\n\\
    Output: tuple - SDK, ErrorCode\\n"); """,
    
    """bp::def( "NxCreatePhysicsSDKWithID", &::NxUtility_NxCreatePhysicsSDKWithID,
    ( bp::arg("sdkVersion"), bp::arg("companyNameStr"), bp::arg("appNameStr"), bp::arg("appVersionStr"), bp::arg("appUserDefinedStr"), bp::arg("allocator")=bp::object(), bp::arg("outputStream")=bp::object(), bp::arg("desc")=::NxPhysicsSDKDesc( ) ),
    "Python-Ogre Hand Wrapped (PhysX) Modified Function Call\\n\\
    Input: \\n\\
    Output: tuple - SDK, ErrorCode\\n"); """

    ] 
    
WRAPPER_DEFINITION_NxActor = \
"""
bp::object NxActor_getShapes(  ) {
    // need to do something here        
    }
"""
WRAPPER_REGISTRATION_NxActor = [
    'def( "getShapes", &::NxActor_getShapes,\
    "Python-Ogre Hand Wrapped\\n";'
#     ,\
#     bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());',  
    ] 
    
WRAPPER_WRAPPER_DEF_NxUtilLib=\
"""    
    virtual ::NxI32 const * NxGetBoxEdgesAxes(  ){
        throw std::logic_error("Error W1049: NxUtilLib::NxGetBoxEdgesAxes needs wrapping!");
//        bp::override func_NxGetBoxEdgesAxes = this->get_override( "NxGetBoxEdgesAxes" );
 //       return func_NxGetBoxEdgesAxes(  );
    }
""" 

WRAPPER_WRAPPER_DEF_NxForceField=\
"""    
    virtual void releaseShape( ::NxForceFieldShape const & arg0 ){
        throw std::logic_error("Error W1049: NxForceField::releaseShape needs wrapping!");
       // bp::override func_releaseShape = this->get_override( "releaseShape" );
        //func_releaseShape( boost::ref(arg0) );
    }
"""  
  
WRAPPER_WRAPPER_DEF_NxHeightFieldShape=\
"""    
    virtual bool overlapAABBTrianglesDeprecated( ::NxBounds3 const & bounds, ::NxU32 flags, ::NxU32 & nb, unsigned int const * & indices ) const {
        throw std::logic_error("Error W1049: NxHeightFieldShape::overlapAABBTrianglesDeprecated needs wrapping!");
//        bp::override func_overlapAABBTrianglesDeprecated = this->get_override( "overlapAABBTrianglesDeprecated" );
//        return func_overlapAABBTrianglesDeprecated( boost::ref(bounds), flags, nb, boost::ref(indices) );
    }
"""
    
WRAPPER_WRAPPER_DEF_NxCookingInterface=\
"""
    virtual bool NxCreatePMap( ::NxPMap & pmap, ::NxTriangleMesh const & mesh, ::NxU32 density, ::NxUserOutputStream * outputStream=0 ){
        throw std::logic_error("Error W1049: NxCookingIterface::NxCreatePMap needs wrapping!");
//        bp::override func_NxCreatePMap = this->get_override( "NxCreatePMap" );
//        return func_NxCreatePMap( boost::ref(pmap), boost::ref(mesh), density, boost::python::ptr(outputStream) );
    }
""" 
  
WRAPPER_WRAPPER_DEF_NxImplicitScreenMesh=\
"""     virtual ::NxReal const * getProjectionMatrix(  ) const {
        throw std::logic_error("Error W1049: NxImplicitScreenMesh::getProjectionMatrix needs wrapping!");
//        bp::override func_getProjectionMatrix = this->get_override( "getProjectionMatrix" );
//        return func_getProjectionMatrix(  );
    }
"""   

WRAPPER_DEFINITION_NxSimpleTriangleMesh=\
""" 
void NxSimpleTriangleMesh_setPoints(::NxSimpleTriangleMesh& me,boost::python::list vertices, unsigned int vertex_count  ){ 

    unsigned int index; 
    ::NxVec3* verts = new NxVec3[vertex_count]; 
    for (index =0 ; index < vertex_count ; index ++) { 
        verts[index] = bp::extract<::NxVec3> (vertices[index]); 
    } 
    me.points = verts;    
} 
void NxSimpleTriangleMesh_setFaces(::NxSimpleTriangleMesh& me,boost::python::list faces, unsigned int face_count  ) { 
    unsigned int index; 
    ::NxU32* triangles = new NxU32[face_count]; 
    for (index =0 ; index < face_count ; index ++) { 
        triangles[index] = (::NxU32)(bp::extract<float> (faces[index])); 
    } 
    me.triangles = triangles;    
} 
""" 

WRAPPER_REGISTRATION_NxSimpleTriangleMesh = [
    """def( "setPoints", &::NxSimpleTriangleMesh_setPoints, 
    ( bp::arg("vertices"), bp::arg("vertex_count")), 
    "Python-Ogre Hand Wrapped\\nSet's the points for a SimpleTriangleMesh", 
     bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""", 
    """def( "setFaces", &::NxSimpleTriangleMesh_setFaces, 
    ( bp::arg("faces"), bp::arg("face_count")), 
    "Python-Ogre Hand Wrapped\\nSet's the faces for a SimpleTriangleMesh", 
     bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""", 

    ] 



 
ConstPointerReference=\
"""     
virtual %(FuncDecl)s {
        throw std::logic_error("Error W1049: %(classname)s::%(function_name)s needs wrapping!");
    }
"""      
PointerReg=\
"""
def( "%(function_name)s", &::%(classname)s_%(function_name)s,"Python-Ogre Hand Wrapped\\n");
    """
PointerDec=\
"""
 boost::python::list 
 %(classname)s_%(function_name)s( %(classname)s & me ) {
    // Probably a better way to do this :) 
    boost::python::list outlist;
    int index;
    int size  = (int)me.%(getsizefunction)s();
    %(returnbase)s * Var_%(function_name)s = me.%(function_name)s();
    
    for (index=0;index<size;index++ ) {
        outlist.append ( boost::python::object( boost::ref(*Var_%(function_name)s++)) );
        //outlist.append ( *Var_%(function_name)s++ );
        }
    return outlist;   
    }
"""     
PointerWrapper=\
"""
virtual %(returnType)s  %(function_name)s(  ) %(const)s {
    bp::override func_%(function_name)s = this->get_override( "%(function_name)s" );
    %(returnSomething)s func_%(function_name)s( );
    }
"""


UserDataDec=\
"""
void %(classname)s_setUserData ( ::%(classname)s& me, PyObject * data) {
    me.userData = data;
    }
    
PyObject * %(classname)s_getUserData ( ::%(classname)s& me) {
    void *  data = me.userData;
    if ( data == NULL) {
        Py_INCREF(Py_None);
        return Py_None;
        }    
    else {
        Py_INCREF( (PyObject *) data );     // I'm passing a reference to this object so better inc the ref :)
        return  (PyObject *) data;
        }    
    }
"""

UserDataReg=[
            """def( "setUserData", &::%(classname)s_setUserData,"Python-Ogre Hand Wrapped\\n");""",    
            """def( "getUserData", &::%(classname)s_getUserData,"Python-Ogre Hand Wrapped\\n");"""
            ]



#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):

    global_ns = mb.global_ns
    
    mb.add_declaration_code( WRAPPER_DEFINITION_Utility )
    apply_reg (mb,  WRAPPER_REGISTRATION_Utility )

    # I'm going to do some automated hand wrapping here 
    getSizeFunctions={'getEmitters':'getNbEmitters',  'getScreenSurfaceMeshes':'getNbScreenSurfaceMeshes',
        'getClothMeshes':'getNbClothMeshes', 'getSoftBodyMeshes':'getNbSoftBodyMeshes',
        'getShapes':'getNbShapes','getForceFields':'getNbForceFields', 'getActors':'getNbActors',
        'getFluids':'getNbFluids','getImplicitScreenMeshes':'getNbImplicitScreenMeshes',
        'getCloths':'getNbCloths','getSoftBodies':'getNbSoftBodies'
        }
#     ignoreClasses = ['NxPhysicsSDK']        
    ignoreClasses = []        
    for f in global_ns.member_functions():
        if f.parent.name in ignoreClasses:
            continue
        for a in f.arguments:
            if "unsigned int const * &" in a.type.decl_string:
                f.exclude()
                if f.parent.is_wrapper_needed():
                    s,m,e=f.decl_string.split('(') # split on open brace to get the variables etc into 'e'
                    FuncDecl = f.return_type.decl_string + " " + f.name + "( " + e 
                    wrapper_code = ConstPointerReference % {'classname':f.parent.name, 'function_name':f.name,
                                    'FuncDecl':FuncDecl }
                    f.parent.add_wrapper_code ( wrapper_code )
                
        if "* const *" in f.return_type.decl_string or "* *" in f.return_type.decl_string :
#             print f.decl_string
#             print dir(f)
#             sys.exit()
            if "const" in  f.decl_string or "const" in f.return_type.decl_string:
                const = "const"
            else:
                const  = ''
            funcname= f.name
            class_ = f.parent
            classname = class_.name
            retbase = f.return_type.base.decl_string
            f.exclude()
            if getSizeFunctions.has_key(funcname):
                getsizefunction = getSizeFunctions[funcname]
                returnType = f.return_type.decl_string
                if 'void' in returnType or returnType == '' :
                    returnSomething = ''
                else:
                    returnSomething = 'return'
                values = { 'returnType':returnType, 'function_name':funcname, 
                    'classname':classname, 'getsizefunction' : getsizefunction,
                    'returnbase':retbase, 'returnSomething':returnSomething,
                    'const':const }
                    
                if f.parent.is_wrapper_needed:
                    wrapper_code = PointerWrapper % values
                    class_.add_wrapper_code( wrapper_code )
                regcode = PointerReg % values
                deccode = PointerDec % values
                class_.add_declaration_code( deccode )
                class_.add_registration_code( regcode )
                print "Wrapped:", f, values
            else:
                print "Excluded:",f
                
    # Patch from Dermont to make userData more useful and simple to use
    vars = ["userData"] 
    for var in global_ns.variables ():
        class_ = var.parent
        classname = class_.name
        if var.name in vars:
            if var.type.decl_string.startswith('void'):
                values = { 'classname':classname }
                for i in range(len(UserDataReg)):
                    regcode = UserDataReg[i] % values
                    class_.add_registration_code( regcode )
                deccode = UserDataDec % values
                class_.add_declaration_code( deccode )
                print "Hand Wrapped:", var.name, values
                 

    rt = mb.class_( 'NxUtilLib' )
    rt.add_wrapper_code(WRAPPER_WRAPPER_DEF_NxUtilLib)
    rt = mb.class_( 'NxCookingInterface' )
    rt.add_wrapper_code(WRAPPER_WRAPPER_DEF_NxCookingInterface)
    rt = mb.class_( 'NxForceField' )
    rt.add_wrapper_code(WRAPPER_WRAPPER_DEF_NxForceField)
    
    rt = mb.class_( 'NxForceFieldShapeGroup' )
    rt.add_wrapper_code( WRAPPER_WRAPPER_DEF_NxForceFieldShapeGroup )

        
    rt = mb.class_( 'NxScene' )
    rt.add_declaration_code( WRAPPER_DEFINITION_NxScene )
    rt.add_wrapper_code( WRAPPER_WRAPPER_DEF_NxScene )
    apply_reg (rt,  WRAPPER_REGISTRATION_NxScene )

#     rt = mb.class_( 'NxStream' )
#     rt.add_declaration_code( WRAPPER_DEFINITION_NxStream )
#     rt.add_wrapper_code( WRAPPER_WRAPPER_DEF_NxStream )
#     apply_reg (rt,  WRAPPER_REGISTRATION_NxStream )
    
    rt = mb.class_( 'NxCompartment' )
    rt.add_declaration_code( WRAPPER_DEFINITION_NxCompartment )
    apply_reg (rt,  WRAPPER_REGISTRATION_NxCompartment )
    
    rt = mb.class_( 'NxConvexMesh' )
    rt.add_declaration_code( WRAPPER_DEFINITION_NxConvexMesh )
    apply_reg (rt,  WRAPPER_REGISTRATION_NxConvexMesh )
    
    rt = mb.class_( 'NxTriangleMesh' )
    rt.add_declaration_code( WRAPPER_DEFINITION_NxTriangleMesh )
    apply_reg (rt,  WRAPPER_REGISTRATION_NxTriangleMesh )
    
    rt = mb.class_( 'NxPhysicsSDK' )
    rt.add_wrapper_code( WRAPPER_WRAPPER_DEF_NxPhysicsSDK )
    rt.add_declaration_code( WRAPPER_DEFINITION_NxPhysicsSDK )
    apply_reg (rt,  WRAPPER_REGISTRATION_NxPhysicsSDK )
    
#     rt = mb.class_( 'NxHeightFieldShape' )
#     rt.add_wrapper_code(WRAPPER_WRAPPER_DEF_NxHeightFieldShape)
#     rt = mb.class_( 'NxImplicitScreenMesh' )
#     rt.add_wrapper_code(WRAPPER_WRAPPER_DEF_NxImplicitScreenMesh)

# #       virtual ::NxActiveTransform * getActiveTransforms( ::NxU32 & nbTransformsOut ) {
# #         namespace bpl = boost::python;
# #         if( bpl::override func_getActiveTransforms = this->get_override( "getActiveTransforms" ) ){
# #             bpl::object py_result = bpl::call<bpl::object>( func_getActiveTransforms.ptr(), nbTransformsOut );
# #             nbTransformsOut = boost::python::extract< unsigned int >( pyplus_conv::get_out_argument( py_result, "nbTransformsOut" ) );
# #             return bpl::extract< ::NxActiveTransform * >( pyplus_conv::get_out_argument( py_result, 0 ) );
# #         }
# #         else{
# #             return NxScene::getActiveTransforms( nbTransformsOut );
# #         }
# #     }
# #     
# #     static boost::python::tuple default_getActiveTransforms( ::NxScene & inst, unsigned int nbTransformsOut ){
# #         NxActiveTransform * result;
# #         if( dynamic_cast< NxScene_wrapper * >( boost::addressof( inst ) ) ){
# #             result = inst.::NxScene::getActiveTransforms(nbTransformsOut);
# #         }
# #         else{
# #             result = inst.getActiveTransforms(nbTransformsOut);
# #         }
# #         typedef bp::return_value_policy< bp::reference_existing_object > call_policies_t;
# #         return bp::make_tuple( pyplusplus::call_policies::make_object< call_policies_t, ::NxActiveTransform * >( result )
# #                                 , nbTransformsOut );
# #     }
  
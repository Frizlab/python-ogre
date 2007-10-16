import os
import environment

WRAPPER_DEFINITION_NxActor_Wrapper = \
"""
virtual ::NxShape * const * getShapes(  ) const {
    bp::override func_getShapes = this->get_override( "getShapes" );
    func_getShapes( );
    }
"""

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
ConstPointerReference=\
"""     
virtual %(FuncDecl)s {
        throw std::logic_error("Error W1049: %(classname)s::%(function_name)s needs wrapping!");
    }
"""      
PointerReg=\
"""
def( "%(function_name)s", &::%(classname)s_%(function_name)s,\
    "Python-Ogre Hand Wrapped\\n");
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
        outlist.append ( *Var_%(function_name)s++ );
        }
    return outlist;   
    }
"""     
PointerWrapper = \
"""
virtual %(returnType)s  %(function_name)s(  ) const {
    bp::override func_%(function_name)s = this->get_override( "%(function_name)s" );
    func_%(function_name)s( );
    }
"""

#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):

    global_ns = mb.global_ns

    # I'm going to do some automated hand wrapping here 
    getSizeFunctions={'getEmitters':'getNbEmitters',  'getScreenSurfaceMeshes':'getNbScreenSurfaceMeshes',
        'getClothMeshes':'getNbClothMeshes', 'getSoftBodyMeshes':'getNbSoftBodyMeshes',
        'getShapes':'getNbShapes','getForceFields':'getNbForceFields', 'getActors':'getNbActors',
        'getFluids':'getNbFluids','getImplicitScreenMeshes':'getNbImplicitScreenMeshes',
        'getCloths':'getNbCloths','getSoftBodies':'getNbSoftBodies'
        }
    for f in global_ns.member_functions():
    
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
            funcname= f.name
            class_ = f.parent
            classname = class_.name
            retbase = f.return_type.base.decl_string
            f.exclude()
            if getSizeFunctions.has_key(funcname):
                getsizefunction = getSizeFunctions[funcname]
                returnType = f.return_type.decl_string
                values = { 'returnType':returnType, 'function_name':funcname, 
                    'classname':classname, 'getsizefunction' : getsizefunction,
                    'returnbase':retbase }
                    
                if f.parent.is_wrapper_needed:
                    wrapper_code = PointerWrapper % values
                    class_.add_wrapper_code( wrapper_code )
                regcode = PointerReg % values
                deccode = PointerDec % values
                class_.add_declaration_code( deccode )
                class_.add_registration_code( regcode )
                print "Wrapped:", f
            else:
                print "Excluded:",f


    rt = mb.class_( 'NxUtilLib' )
    rt.add_wrapper_code(WRAPPER_WRAPPER_DEF_NxUtilLib)
    rt = mb.class_( 'NxCookingInterface' )
    rt.add_wrapper_code(WRAPPER_WRAPPER_DEF_NxCookingInterface)
    rt = mb.class_( 'NxForceField' )
    rt.add_wrapper_code(WRAPPER_WRAPPER_DEF_NxForceField)
#     rt = mb.class_( 'NxHeightFieldShape' )
#     rt.add_wrapper_code(WRAPPER_WRAPPER_DEF_NxHeightFieldShape)
    rt = mb.class_( 'NxImplicitScreenMesh' )
    rt.add_wrapper_code(WRAPPER_WRAPPER_DEF_NxImplicitScreenMesh)

    
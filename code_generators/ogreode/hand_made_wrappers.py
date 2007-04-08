import os
import environment

WRAPPER_DEFINITION_OgreOdeLoader=\
"""        
boost::python::object OgreOde_Loader_loadObject(OgreOde_Loader::DotLoader& me, const Ogre::String &filename, 
                    const Ogre::String &object_name, const Ogre::String &instance_name = Ogre::StringUtil::BLANK)
{
    OgreOde_Prefab::Object *  r = me.loadObject( filename, object_name, instance_name );
   if( dynamic_cast< OgreOde_Prefab::Vehicle* >( r ) ){
        return boost::python::object( (OgreOde_Prefab::Vehicle *)( r ) );
    }
    if( dynamic_cast< OgreOde_Prefab::Ragdoll* >( r ) ){
        return boost::python::object( (OgreOde_Prefab::Ragdoll *)( r ) );
    }
    return boost::python::object( r ); //unknown type
}
"""     
WRAPPER_REGISTRATION_OgreOdeLoader = [
    'def( "loadObject", &::OgreOde_Loader_loadObject,\
    ( bp::arg("filename"), bp::arg("object_name"), bp::arg("instance_name")=Ogre::StringUtil::BLANK ),\
    "Python-Ogre Hand Wrapped\\nReturns a vehicle or ragdoll as appropiate" );',
    ]
    
def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    rt = mb.class_( 'DotLoader' )
    rt.add_declaration_code( WRAPPER_DEFINITION_OgreOdeLoader )
    apply_reg (rt,  WRAPPER_REGISTRATION_OgreOdeLoader )
       
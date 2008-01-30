import os
import environment

WRAPPER_DEFINITION_TreeLoader2D = \
"""
static PyObject*  mSubscriber_tree;
static std::string mName_tree;
Ogre::Real HelperHeightFunction_Tree2D ( Ogre::Real x, Ogre::Real z, void *userData) {
    Ogre::Real y;
    if (mName_tree.length() == 0 )
        y = boost::python::call<Ogre::Real>(mSubscriber_tree, x, z, userData);
    else
        y = boost::python::call_method<Ogre::Real>(mSubscriber_tree, mName_tree.c_str(), x, z, userData);
    return y;
    } 

void TreeLoader2D_setHeightFunction( TreeLoader2D & me, PyObject*  function, std::string name=""  ) {
    mSubscriber_tree = function;
    mName_tree = name;
    me.setHeightFunction ( &HelperHeightFunction_Tree2D );
    }    
"""    

WRAPPER_DEFINITION_GrassLoader = \
"""
static PyObject*  mSubscriber_grass;
static std::string mName_grass;
Ogre::Real HelperHeightFunction_Grass ( Ogre::Real x, Ogre::Real z, void * userData) {
    Ogre::Real y;
    if (mName_grass.length() == 0 )
        y = boost::python::call<Ogre::Real>(mSubscriber_grass, x, z, userData );
    else
        y = boost::python::call_method<Ogre::Real>(mSubscriber_grass, mName_grass.c_str(), x, z, userData);
    return y;
    } 

void GrassLoader_setHeightFunction( GrassLoader & me, PyObject*  function, std::string name=""  ) {
    mSubscriber_grass = function;
    mName_grass = name;
    me.setHeightFunction ( &HelperHeightFunction_Grass );
    }    
"""    

WRAPPER_REGISTRATION_TreeLoader2D =[
    'def ("setHeightFunction", &::TreeLoader2D_setHeightFunction );'
    ]
    
WRAPPER_REGISTRATION_GrassLoader =[
    'def ("setHeightFunction", &::GrassLoader_setHeightFunction );'
    ]

    
    
WRAPPER_DEFINITION_PagedGeometry = \
"""
void PagedGeometry_addDetailLevel_ImpostorPage( PagedGeometry & me, Ogre::Real maxRange, Ogre::Real transitionLength = 0) {
    me.addDetailLevel<ImpostorPage>(maxRange, transitionLength);
    }
void PagedGeometry_addDetailLevel_BatchPage( PagedGeometry & me, Ogre::Real maxRange, Ogre::Real transitionLength = 0) {
    me.addDetailLevel<BatchPage>(maxRange, transitionLength);
    }
void PagedGeometry_addDetailLevel_GrassPage( PagedGeometry & me, Ogre::Real maxRange, Ogre::Real transitionLength = 0) {
    me.addDetailLevel<GrassPage>(maxRange, transitionLength);
    } 
"""
WRAPPER_REGISTRATION_PagedGeometry =[
    'def ("addDetailLevelBatchPage", &::PagedGeometry_addDetailLevel_BatchPage,\
        (bp::arg("maxRange"), bp::arg("transitionLength")=0.0 ));'
    ,'def ("addDetailLevelImpostorPage", &::PagedGeometry_addDetailLevel_ImpostorPage,\
        (bp::arg("maxRange"), bp::arg("transitionLength")=0.0 ));'
    ,'def ("addDetailLevelGrassPage", &::PagedGeometry_addDetailLevel_GrassPage,\
        (bp::arg("maxRange"), bp::arg("transitionLength")=0.0 ));'
    ]


#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    rt = mb.class_( 'GrassLoader' )
    rt.add_declaration_code( WRAPPER_DEFINITION_GrassLoader )
    apply_reg (rt, WRAPPER_REGISTRATION_GrassLoader )

    rt = mb.class_( 'TreeLoader2D' )
    rt.add_declaration_code( WRAPPER_DEFINITION_TreeLoader2D )
    apply_reg (rt, WRAPPER_REGISTRATION_TreeLoader2D )
    
    rt = mb.class_( 'PagedGeometry' )
    rt.add_declaration_code( WRAPPER_DEFINITION_PagedGeometry )
    apply_reg (rt, WRAPPER_REGISTRATION_PagedGeometry )

    
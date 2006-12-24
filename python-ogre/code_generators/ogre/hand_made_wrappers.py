import os
import environment

WRAPPER_DEFINITION_General = \
"""
boost::python::tuple 
GetOgreVersion () {
            return ( boost::python::make_tuple( Ogre::StringConverter::toString(OGRE_VERSION_MAJOR),
                                                Ogre::StringConverter::toString(OGRE_VERSION_MINOR),
                                                Ogre::StringConverter::toString(OGRE_VERSION_PATCH),
                                                OGRE_VERSION_NAME 
                                                ) );
}
boost::python::tuple 
GetPythonOgreVersion () {
            return ( boost::python::make_tuple( """ + environment.PythonOgreMajorVersion + """,
                                                """ + environment.PythonOgreMinorVersion + """,
                                                """ + environment.PythonOgrePatchVersion + """
                                                ) );
}
void
Utility_setFloat(void * ptrin, boost::python::list listin)
{
    int index;
    float * newptr = reinterpret_cast<float *>(ptrin);
    for (index=0;index<len(listin);index++ ) {
//        newptr[index] = boost::python::extract<float> (listin[index]);
        *newptr++ = boost::python::extract<float> (listin[index]);
        }
}

void
Utility_setUint16(void * ptrin, boost::python::list listin)
{
    int index;
    Ogre::uint16 * newptr = reinterpret_cast<Ogre::uint16 *>(ptrin);
    for (index=0;index<len(listin);index++ ) {
        *newptr++ = boost::python::extract<Ogre::uint16> (listin[index]);
        }
}

// sometimes we need to take the ctypess addressof(), an int, and recast it as a general void *
void *
Utility_CastVoidPtr ( int address )
{
    return (void *) address;
    }
    

// void
// Utility_setFloat(void * ptrin, float datain)
// {
//     float * newptr = reinterpret_cast<float *>(ptrin);
//     newptr[0]= datain;
// }
"""            
WRAPPER_REGISTRATION_General = \
"""
    def( "GetOgreVersion", &GetOgreVersion);
    def( "GetPythonOgreVersion", &GetPythonOgreVersion);
    def( "setFloat", &Utility_setFloat );
    def( "setUint16", &Utility_setUint16 );
    bp::def( "CastVoidPtr", &Utility_CastVoidPtr,
                bp::return_value_policy< bp::return_opaque_pointer >());
    
"""

###################################################

WRAPPER_DEFINITION_CombinedListener = \
"""
namespace Ogre {
    class CombinedListener : public Ogre::FrameListener, public Ogre::KeyListener,
                 public Ogre::MouseListener, public Ogre::MouseMotionListener
{
public:
    CombinedListener() {}
    virtual ~CombinedListener() {}
    
    // KeyListener methods
    virtual void keyClicked(Ogre::KeyEvent *e) {}
    virtual void keyPressed(Ogre::KeyEvent *e) {}
    virtual void keyReleased(Ogre::KeyEvent *e) {}
    
    // MouseListener methods
    virtual void mouseClicked(Ogre::MouseEvent *e) {}
    virtual void mousePressed(Ogre::MouseEvent *e) {}
    virtual void mouseReleased(Ogre::MouseEvent *e) {}
    virtual void mouseEntered(Ogre::MouseEvent *e) {}
    virtual void mouseExited(Ogre::MouseEvent *) {}
    
    // MouseMotionListener methods
    virtual void mouseMoved(Ogre::MouseEvent *e) {}
    virtual void mouseDragged(Ogre::MouseEvent *e) {}
    
    // FrameEvent methods
    virtual bool frameStarted (const FrameEvent &evt) {return true;}
    virtual bool frameEnded (const FrameEvent &evt) {return true;}
};
}
"""


WRAPPER_DEFINITION_ConfigFile = \
"""
// We don't currently support multimaps very well so to extract the resources from a config file
// we call this with the multimap and it extracts it into a list of tuples for us
boost::python::list
ConfigFile_getMultiMapSettings ( Ogre::ConfigFile& cf, Ogre::ConfigFile::SettingsMultiMap * settings ){
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    Ogre::String typeName, archName;
    boost::python::list retlist = boost::python::list();
    for (i = settings->begin(); i != settings->end(); ++i)
    {
        typeName = i->first;
        archName = i->second;
        retlist.append(boost::python::make_tuple(typeName, archName));
    }
    return ( retlist );
}
"""
WRAPPER_REGISTRATION_ConfigFile = \
"""
    def( "getMultiMapSettings", &::ConfigFile_getMultiMapSettings )
"""

#####################################################################

WRAPPER_DEFINITION_RenderTarget = \
"""
static int
RenderTarget_getCustomAttributeInt(Ogre::RenderTarget& rd, const std::string& name) {
    int value(0);
    rd.getCustomAttribute( name, &value );
    return value;
}
"""
WRAPPER_REGISTRATION_RenderTarget = \
"""
    def( "getCustomAttributeInt", &::RenderTarget_getCustomAttributeInt );
"""

#####################################################################

WRAPPER_DEFINITION_HardwareBufferManager = \
"""
::Ogre::HardwareIndexBufferSharedPtr 
hbm_createIndexBuffer( ::Ogre::HardwareIndexBuffer::IndexType itype, ::size_t numIndexes, 
                        ::Ogre::HardwareBuffer::Usage usage, bool useShadowBuffer=false ){
        return me.createIndexBuffer( itype, numIndexes, usage, useShadowBuffer );
    }

"""
WRAPPER_REGISTRATION_HardwareBufferManager = \
"""
    def( "createIndexBuffer", &::hbm_createIndexBuffer );
//           bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
"""

#########################################

WRAPPER_DEFINITION_RenderWindow = \
"""
boost::python::tuple 
RenderWindow_getMetrics( Ogre::RenderWindow& rw ) {
   unsigned int width, height, colourDepth;
   int left, top;
   rw.getMetrics(  width,  height, colourDepth,  left,  top );
   return ( boost::python::make_tuple(width, height, colourDepth, left, top));
}
"""
WRAPPER_REGISTRATION_RenderWindow = \
"""
    def( "getMetrics", &::RenderWindow_getMetrics );
"""

## Needed as boost doesn't handle this overload properly and always uses 'Plane' as the override - see renderToTexture demo
WRAPPER_DEFINITION_Frustum = \
"""
void
Frustum_enableCustomNearClipPlaneMP(Ogre::Frustum& Fr, Ogre::MovablePlane const * MP ) {
        Fr.enableCustomNearClipPlane ( MP ) ;
}
"""
WRAPPER_REGISTRATION_Frustum = \
"""
    def( "enableCustomNearClipPlaneMP", &::Frustum_enableCustomNearClipPlaneMP );
"""

if environment.ogre.version != "CVS":
    ### need to override keylistener etc
    WRAPPER_DEFINITION_EventProcessor =\
    """
	void EventProcessor_addKeyListener(Ogre::EventProcessor& evp, Ogre::KeyListener  * l) {
	    //evp.addKeyListener ( l );
	    }
	void EventProcessor_addMouseListener(Ogre::EventProcessor& evp, Ogre::MouseListener  * m) {
	    //evp.addMouseListener (  m );
	    }

	"""     
    WRAPPER_REGISTRATION_EventProcessor = \
    """
        def( "addKeyListenerFunc", &::EventProcessor_addKeyListener );
        EventProcessor_exposer.def( "addMouseListenerFunc", &::EventProcessor_addMouseListener );
    """
WRAPPER_DEFINITION_SubMesh =\
"""
void
SubMesh_createVertexData ( Ogre::SubMesh & me ) {
    me.vertexData = new Ogre::VertexData();
    }
 
"""
WRAPPER_REGISTRATION_SubMesh = \
"""
    def( "createVertexData", &::SubMesh_createVertexData );
""" 

WRAPPER_DEFINITION_MeshManager =\
"""
// Needs a void* to a databuffer (controlPointBuffer) - we want to use ctypes within python
// and pass 'addressof()' to this function - so we get an address in an int
Ogre::PatchMeshPtr 
MeshManager_createBezierPatch(::Ogre::MeshManager & me,
            const Ogre::String& name, const Ogre::String& groupName, int controlPointBuffer, 
            Ogre::VertexDeclaration *declaration, size_t width, size_t height,
            size_t uMaxSubdivisionLevel = Ogre::PatchSurface::AUTO_LEVEL, 
            size_t vMaxSubdivisionLevel = Ogre::PatchSurface::AUTO_LEVEL,
            Ogre::PatchSurface::VisibleSide visibleSide = Ogre::PatchSurface::VS_FRONT,
            Ogre::HardwareBuffer::Usage vbUsage = Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, 
            Ogre::HardwareBuffer::Usage ibUsage = Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY,
            bool vbUseShadow = true, bool ibUseShadow = true) {
            
       return me.createBezierPatch(
             name, groupName, (void*) controlPointBuffer, 
            declaration,  width,  height,
            uMaxSubdivisionLevel, 
            vMaxSubdivisionLevel,
            visibleSide,
            vbUsage, 
            ibUsage,
            vbUseShadow, ibUseShadow );
            }
"""
WRAPPER_REGISTRATION_MeshManager = \
"""
    def( "createBezierPatchSpecial", &::MeshManager_createBezierPatch );
"""
		
WRAPPER_DEFINITION_Mesh =\
"""
boost::python::tuple
Mesh_suggestTangentVectorBuildParams(::Ogre::Mesh & me, ::Ogre::VertexElementSemantic targetSemantic) {
unsigned short a, b;
bool ret;

    ret = me.suggestTangentVectorBuildParams(targetSemantic,
		a, b);
		return boost::python::make_tuple ( ret, a, b );
}
"""     
WRAPPER_REGISTRATION_Mesh = \
"""
    def( "suggestTangentVectorBuildParams", &::Mesh_suggestTangentVectorBuildParams );
"""

WRAPPER_DEFINITION_Viewport =\
"""
boost::python::tuple
Viewport_getActualDimensions(::Ogre::Viewport & me ) {
    int l, t, w, h;
    me.getActualDimensions(l, t, w, h);
    return boost::python::make_tuple ( l, t, w, h );
    }
"""
WRAPPER_REGISTRATION_Viewport = \
"""
    def( "getActualDimensions", &::Viewport_getActualDimensions );
"""

def apply( mb ):
    rt = mb.class_( 'ConfigFile' )
    rt.add_declaration_code( WRAPPER_DEFINITION_ConfigFile )
    rt.add_registration_code( WRAPPER_REGISTRATION_ConfigFile )
    rt = mb.class_( 'RenderTarget' )
    rt.add_declaration_code( WRAPPER_DEFINITION_RenderTarget )
    rt.add_registration_code( WRAPPER_REGISTRATION_RenderTarget )
#     rt = mb.class_( 'RenderWindow' )
#     rt.add_declaration_code( WRAPPER_DEFINITION_RenderWindow )
#     rt.add_registration_code( WRAPPER_REGISTRATION_RenderWindow )
    rt = mb.class_( 'Frustum' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Frustum )
    rt.add_registration_code( WRAPPER_REGISTRATION_Frustum )
    if environment.ogre.version != "CVS":
        rt = mb.class_( 'EventProcessor' )
        rt.add_declaration_code( WRAPPER_DEFINITION_EventProcessor )
        rt.add_registration_code( WRAPPER_REGISTRATION_EventProcessor )
        ## now add support for the combined listener
        rt = mb.class_( 'CombinedListener' )
        rt.add_declaration_code( WRAPPER_DEFINITION_CombinedListener )
    rt = mb.class_( 'Mesh' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Mesh )
    rt.add_registration_code( WRAPPER_REGISTRATION_Mesh )
    rt = mb.class_( 'MeshManager' )
    rt.add_declaration_code( WRAPPER_DEFINITION_MeshManager )
    rt.add_registration_code( WRAPPER_REGISTRATION_MeshManager )
#     rt = mb.class_( 'HardwareBufferManager' )
#     rt.add_declaration_code( WRAPPER_DEFINITION_HardwareBufferManager )
#     rt.add_registration_code( WRAPPER_REGISTRATION_HardwareBufferManager )
    rt = mb.class_( 'SubMesh' )
    rt.add_declaration_code( WRAPPER_DEFINITION_SubMesh )
    rt.add_registration_code( WRAPPER_REGISTRATION_SubMesh )
        
        
    mb.add_declaration_code( WRAPPER_DEFINITION_General )
    mb.add_registration_code( WRAPPER_REGISTRATION_General )
#     rt.add_declaration_code( WRAPPER_DEFINITION_VertexBuffer )
#     rt.add_registration_code( WRAPPER_REGISTRATION_VertexBuffer )
    


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
Utility_setUint16(void * ptrin, boost::python::list listin)     // unsigned short
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
    bp::def( "GetOgreVersion", &GetOgreVersion,
                "Python-Ogre Helper Function: Return the version of Ogre.\\n\\
                Input: None\\n\\
                Ouput: Tuple [major,minor,patch]");
    bp::def( "GetPythonOgreVersion", &GetPythonOgreVersion,
                "Python-Ogre Helper Function: Return the version of Python-Ogre.\\n\\
                Input: None\\n\\
                Ouput: Tuple [major,minor,patch]");
    bp::def( "setFloat", &Utility_setFloat,
                "Python-Ogre Helper Function: Write Floats to Memory.\\n\\
                Input: void *, Python List (numerics)\\n\\
                Ouput: None\\n\\
                The list is extracted as floats and written to memory starting at the pointer" );
    bp::def( "setUint16", &Utility_setUint16,
                "Python-Ogre Helper Function: Write Unsigned Ints to Memory.\\n\\
                Input: void *, Python List (numerics)\\n\\
                Ouput: None\\n\\
                The list is extracted as unsigned ints and written to memory starting at the pointer" );
    bp::def( "CastVoidPtr", &Utility_CastVoidPtr,
                bp::return_value_policy< bp::return_opaque_pointer >(),
                "Python-Ogre Helper Function: Casts a number to a void *.\\n\\
                Input: numeric value (typically CTypes.addressof(xx) )\\n\\
                Output: A void pointer with the input address");
                
       
    
"""

##################################################################

WRAPPER_DEFINITION_BillboardParticleRenderer = \
"""
boost::python::str
BBP_getType(Ogre::BillboardParticleRenderer & me) {
    return boost::python::str( me.getType() );
    }
"""
WRAPPER_REGISTRATION_BillboardParticleRenderer = \
"""
    def( "getType", &::BBP_getType );
"""

WRAPPER_DEFINITION_BillboardParticleRendererFactory = \
"""
boost::python::str
BBPF_getType(Ogre::BillboardParticleRenderer & me) {
    return boost::python::str( me.getType() );
    }
"""
WRAPPER_REGISTRATION_BillboardParticleRendererFactory = \
"""
    def( "getType", &::BBPF_getType );
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


# # WRAPPER_DEFINITION_ConfigFile = \
# # """
# # // We don't currently support multimaps very well so to extract the resources from a config file
# # // we call this with the multimap and it extracts it into a list of tuples for us
# # boost::python::list
# # ConfigFile_getMultiMapSettings ( Ogre::ConfigFile& cf, Ogre::ConfigFile::SettingsMultiMap * settings ){
# #     Ogre::ConfigFile::SettingsMultiMap::iterator i;
# #     Ogre::String typeName, archName;
# #     boost::python::list retlist = boost::python::list();
# #     for (i = settings->begin(); i != settings->end(); ++i)
# #     {
# #         typeName = i->first;
# #         archName = i->second;
# #         retlist.append(boost::python::make_tuple(typeName, archName));
# #     }
# #     return ( retlist );
# # }
# # """
# # WRAPPER_REGISTRATION_ConfigFile = \
# # """
# #     def( "getMultiMapSettings", &::ConfigFile_getMultiMapSettings,
# #     "Python-Ogre Helper Function: Return Configfile settings as a list" )
# # """

#####################################################################

WRAPPER_DEFINITION_RenderTarget = \
"""
static int
RenderTarget_getCustomAttributeInt(Ogre::RenderTarget& rd, const std::string& name) {
    int value(0);
    rd.getCustomAttribute( name, &value );
    return value;
}
static float
RenderTarget_getCustomAttributeFloat(Ogre::RenderTarget& rd, const std::string& name) {
    float value(0);
    rd.getCustomAttribute( name, &value );
    return value;
}
"""
WRAPPER_REGISTRATION_RenderTarget = \
"""
    def( "getCustomAttributeInt", &::RenderTarget_getCustomAttributeInt,
    "Python-Ogre Helper Function: Get a Custom Atribute as an int\\n\\
    Input: None\\n\\
    Output: Numeric (int)\\n\\
    Gets a custom attribure from RenderTarget as an int" );
    
    RenderTarget_exposer.def( "getCustomAttributeFloat", &::RenderTarget_getCustomAttributeFloat,
    "Python-Ogre Helper Function: Get a Custom Atribute as a float\\n\\
    Input: None\\n\\
    Output: Numeric (float)\\n\\
    Gets a custom attribure from RenderTarget as a float" );
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

# # # # #####################################################################

# # # # WRAPPER_DEFINITION_SectionIterator = \
# # # # """
# # # # Ogre::SectionIterator
# # # # SectionIterator_iter ( Ogre::SectionIterator & me ) {
# # # #     return me;
# # # #     }
# # # #    
# # # # boost::python::tuple
# # # # SectionIterator_next( Ogre::SectionIterator & me ){
# # # #     if me.hasMoreElements(){
# # # #         return boost::python::make_tuple (
# # # #                     boost::python::str (me.peekNextKey())
# # # #                    ,boost::python::str (me.getNext())
# # # #                     )
# # # #         }
# # # #     else
# # # #         boost::python::raise StopIteration ()   
# # # #     }
# # # # """
# # # # WRAPPER_REGISTRATION_SectionIterator = \
# # # # """
# # # #        SectionIterator_exposer.def( "__iter__", &::SectionIterator_iter );
# # # #        SectionIterator_exposer.def( "__next__", &::SectionIterator_next );
# # # # """

#########################################

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

## WARNING, this is OLD CODE and is probably broken
if environment.ogre.version == "1.2":
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
    
#################################################################################################
    
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

#################################################################################################
		
# # WRAPPER_DEFINITION_Mesh =\
# # """
# # boost::python::tuple
# # Mesh_suggestTangentVectorBuildParams(::Ogre::Mesh & me, ::Ogre::VertexElementSemantic targetSemantic) {
# # unsigned short a, b;
# # bool ret;

# #     ret = me.suggestTangentVectorBuildParams(targetSemantic,
# # 		a, b);
# # 		return boost::python::make_tuple ( ret, a, b );
# # }
# # """     
# # WRAPPER_REGISTRATION_Mesh = \
# # """
# #     bp::def( "suggestTangentVectorBuildParams", &::Mesh_suggestTangentVectorBuildParams );
# # """


def iter_as_generator_vector( cls ):
    print "ITER:", cls
    
    code = os.linesep.join([ 
            'typedef %(cls)s iter_type;'
            , 'generators::generator_maker_vector< iter_type >::register_< %(call_policies)s >( %(exposer_name)s );'])
    cls.add_registration_code( 
            code % { 'cls' : cls.decl_string
                     , 'call_policies' : cls.mem_fun( 'getNext' ).call_policies.create_type()
                     , 'exposer_name' : cls.class_var_name }
            , works_on_instance=False )
            
def iter_as_generator_map( cls ):
    print "ITER:", cls
    
    code = os.linesep.join([ 
            'typedef %(cls)s iter_type;'
            , 'generators::generator_maker_map< iter_type >::register_< %(call_policies)s >( %(exposer_name)s );'])
    cls.add_registration_code( 
            code % { 'cls' : cls.decl_string
                     , 'call_policies' : cls.mem_fun( 'getNext' ).call_policies.create_type()
                     , 'exposer_name' : cls.class_var_name }
            , works_on_instance=False )

#################################################################################################
#################################################################################################

def apply( mb ):
    rt = mb.class_( 'RenderTarget' )
    rt.add_declaration_code( WRAPPER_DEFINITION_RenderTarget )
    rt.add_registration_code( WRAPPER_REGISTRATION_RenderTarget )
    
    rt = mb.class_( 'Frustum' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Frustum )
    rt.add_registration_code( WRAPPER_REGISTRATION_Frustum )
    
    rt = mb.class_( 'SubMesh' )
    rt.add_declaration_code( WRAPPER_DEFINITION_SubMesh )
    rt.add_registration_code( WRAPPER_REGISTRATION_SubMesh )

#     rt = mb.class_( 'Mesh' )
#     rt.add_declaration_code( WRAPPER_DEFINITION_Mesh )
#     rt.add_registration_code( WRAPPER_REGISTRATION_Mesh )
    
#     rt = mb.class_( 'BillboardParticleRenderer' )
#     rt.add_declaration_code( WRAPPER_DEFINITION_BillboardParticleRenderer )
#     rt.add_registration_code( WRAPPER_REGISTRATION_BillboardParticleRenderer )
#     
#     rt = mb.class_( 'BillboardParticleRendererFactory' )
#     rt.add_declaration_code( WRAPPER_DEFINITION_BillboardParticleRendererFactory )
#     rt.add_registration_code( WRAPPER_REGISTRATION_BillboardParticleRendererFactory )
      
#     rt = mb.class_( 'MapIterator' )
#     rt.add_declaration_code( WRAPPER_DEFINITION_SectionIterator )
#     rt.add_registration_code( WRAPPER_REGISTRATION_SectionIterator )
#     rt = mb.class_( 'ConfigFile' )
#     rt.add_declaration_code( WRAPPER_DEFINITION_ConfigFile )
#     rt.add_registration_code( WRAPPER_REGISTRATION_ConfigFile )

    mb.add_declaration_code( WRAPPER_DEFINITION_General )
    mb.add_registration_code( WRAPPER_REGISTRATION_General )
    
    vec_iterators = mb.classes( lambda cls: cls.name.startswith( 'VectorIterator<' ) )
    for cls in vec_iterators:
        iter_as_generator_vector( cls )
        
    map_iterators = mb.classes( lambda cls: cls.name.startswith( 'MapIterator<' ) )
    for cls in map_iterators:
        iter_as_generator_map( cls )   
             
    if environment.ogre.version == "1.2":
        rt = mb.class_( 'EventProcessor' )
        rt.add_declaration_code( WRAPPER_DEFINITION_EventProcessor )
        rt.add_registration_code( WRAPPER_REGISTRATION_EventProcessor )
        ## now add support for the combined listener
        rt = mb.class_( 'CombinedListener' )
        rt.add_declaration_code( WRAPPER_DEFINITION_CombinedListener )        

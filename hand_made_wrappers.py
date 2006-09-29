import os
import environment

WRAPPER_DEFINITION_RenderTarget = \
"""
static int
RenderTarget_getCustomAttributeWindowInt(Ogre::RenderTarget& rd, const std::string& name) {
    int value(0);
    rd.getCustomAttribute( name, &value );
    return value;
}
"""

WRAPPER_DEFINITION_RenderWindow = \
"""
boost::python::tuple 
RenderWindow_getMetricsIntWrapper( Ogre::RenderWindow& rw ) {
   unsigned int width, height, colourDepth;
   int left, top;
   rw.getMetrics(  width,  height, colourDepth,  left,  top );
   return ( boost::python::make_tuple(width, height, colourDepth, left, top));
}
"""

WRAPPER_REGISTRATION_RenderTarget = \
"""
    def( "getCustomAttributeWindowInt", &::RenderTarget_getCustomAttributeWindowInt )
"""
WRAPPER_REGISTRATION_RenderWindow = \
"""
    def( "getMetricsInt", &::RenderWindow_getMetricsIntWrapper )
"""


def apply( mb ):
    rt = mb.class_( 'RenderTarget' )
    rt.add_declaration_code( WRAPPER_DEFINITION_RenderTarget )
    rt.add_registration_code( WRAPPER_REGISTRATION_RenderTarget )
    rt = mb.class_( 'RenderWindow' )
    rt.add_declaration_code( WRAPPER_DEFINITION_RenderWindow )
    rt.add_registration_code( WRAPPER_REGISTRATION_RenderWindow )


import os
import environment

WRAPPER_DEFINITION = \
"""
static int
RenderTarget_getCustomAttributeWindowInt(Ogre::RenderTarget& rd, const std::string& name) {
    int value(0);
    rd.getCustomAttribute( name, &value );
    return value;
}
"""

WRAPPER_REGISTRATION = \
"""
    def( "getCustomAttributeWindowInt", &::RenderTarget_getCustomAttributeWindowInt )
"""

def apply( mb ):
    rt = mb.class_( 'RenderTarget' )
    rt.add_declaration_code( WRAPPER_DEFINITION )
    rt.add_registration_code( WRAPPER_REGISTRATION )


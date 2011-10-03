import os
import environment


WRAPPER_DEFINITION_RayCaster=\
"""	
boost::python::list
getRaycastHits( NxOgre::RayCaster & me ) {
    boost::python::list l;
	if (me.mReport.count() == 0) {
		return l;
	}
	
	NxOgre::RayCastReport::Iterator rBegin = me.mReport.items.begin();
	NxOgre::RayCastReport::Iterator rEnd = me.mReport.items.end();
	
	for(; rBegin!=rEnd; ++rBegin)
	    l.append ( (*rBegin).second.t  );
	    
    return l;	    
}
"""

WRAPPER_REGISTRATION_RayCaster= [
    """def ("getRaycastHits", &::getRaycastHits,\
                "Python-Ogre Helper Function: Return a list of raycast hits"  );"""
    ]
    
#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    cs = mb.class_( 'RayCaster' )
    cs.add_declaration_code( WRAPPER_DEFINITION_RayCaster )
    apply_reg (cs,  WRAPPER_REGISTRATION_RayCaster )
    
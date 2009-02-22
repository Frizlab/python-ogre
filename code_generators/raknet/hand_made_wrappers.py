import os
import environment
WRAPPER_DEFINITION_General = \
"""
/* not using this as potential issue with assuming null terminated string -- !!!??? */
boost::python::tuple
processPacket( Packet * p )
{
   unsigned char type = 0;
   boost::python::str outstr;
   unsigned long time = 0;
   if (p ) {
      if ((unsigned char)p->data[0] == ID_TIMESTAMP) {
         assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
         time = (unsigned long) p->data[1];
         outstr = boost::python::str( p->data[sizeof(unsigned char) + sizeof(unsigned long)] );
      }
      else {  
         outstr = boost::python::str(p->data[0]);
      }   
      type = (unsigned char) p->data[0];
   }
   return boost::python::make_tuple( type, outstr, time );
}
"""

WRAPPER_REGISTRATION_General = [
    """bp::def( "processPacket", &processPacket,\
    "PythonOgre Helper Function, pass it a packet and get back the identified, data(string) and timestamp if applicible" );"""
    ]
                

WRAPPER_DEFINITION_RakPeerInterface =\
"""
boost::python::tuple
RakPeerInterface_ReceiveList( RakPeerInterface * client )
{
    boost::python::list outlist;
    size_t index = 0;
    unsigned long time;
    Packet * p;
    p = client->Receive();
    if (p ) {
      if ((unsigned char)p->data[0] == ID_TIMESTAMP) {
         assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
         time = (unsigned long) p->data[1];
         index = sizeof(unsigned char) + sizeof(unsigned long); // offset past the timestamp
      }   
      size_t size = p->bitSize / 8; // FIXME on 64 bit systems????
      for (;index<size;index++ ) {
         outlist.append ( (unsigned char) p->data[index] );
         }
      }
    return boost::python::make_tuple(p,outlist,time);
}

"""
WRAPPER_REGISTRATION_RakPeerInterface = [
    """def( "ReceiveList", &RakPeerInterface_ReceiveList,\
       "Python Ogre helper function, returns a list (empty or not) with recieve data")"""
    ]
#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    rt = mb.class_( 'RakPeerInterface' )
    rt.add_declaration_code( WRAPPER_DEFINITION_RakPeerInterface )
    apply_reg (rt,  WRAPPER_REGISTRATION_RakPeerInterface )

#     mb.add_declaration_code( WRAPPER_DEFINITION_General )
#     apply_reg (mb,  WRAPPER_REGISTRATION_General )
#     
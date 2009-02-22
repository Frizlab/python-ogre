import os
import environment
WRAPPER_DEFINITION_General = \
"""

unsigned char GetPacketIdentifier(Packet *p)
{
	if (p==0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	printf ( "KEY: %d %d\n", p->data, p->data[0]);
	return (unsigned char) p->data[0];
}
Packet * p;	
Packet * receivePacket ( RakPeerInterface * client )
{
   p = client->Receive();
   if (p ) GetPacketIdentifier(p);
   return p;
   }
"""

WRAPPER_REGISTRATION_General = [
    """bp::def( "receivePacket", &receivePacket,\
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());""",
    """bp::def( "GetPacketIdentifier", &GetPacketIdentifier );"""
    ]
                


#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    return
    mb.add_declaration_code( WRAPPER_DEFINITION_General )
    apply_reg (mb,  WRAPPER_REGISTRATION_General )
    
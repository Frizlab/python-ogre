import os
import environment

WRAPPER_DEFINITION_netSocket =\
"""
int netSocket_send ( netSocket & me, boost::python::list listin, int flags = 0 )
{
    int index, result;
    int size = len(listin);
    byte * buffer = new byte [size];
    for (index=0;index<size;index++ ) {
        buffer[index] = (byte) boost::python::extract<int> (listin[index]);
    }
    result = me.send((const void *) buffer, size, flags );
    delete buffer;
    return result;
}

int netSocket_send_str ( netSocket & me, boost::python::str listin, int flags = 0 )
{
    int index, result;
    int size = len(listin);
    byte * buffer = new byte[size];
    for (index=0;index<size;index++ ) {
        buffer[index] = (byte) boost::python::extract<char> (listin[index]);
    }
    result = me.send((const void *) buffer, size, flags );
    delete buffer;
    return result;
}
int netSocket_sendto ( netSocket & me, boost::python::list listin, int flags, const netAddress* to )
{
    int index, result;
    int size = len(listin);
    byte * buffer = new byte[size];
    for (index=0;index<size;index++ ) {
        buffer[index] = boost::python::extract<int> (listin[index]);
    }
    result= me.sendto((const void *) buffer, size, flags, to );
    delete buffer;
    return result;
}  
int netSocket_sendto_str ( netSocket & me, boost::python::str listin, int flags, const netAddress* to )
{
    int index, result;
    int size = len(listin);
    byte * buffer = new byte[size];
    for (index=0;index<size;index++ ) {
        buffer[index] = boost::python::extract<char> (listin[index]);
    }
    result= me.sendto((const void *) buffer, size, flags, to );
    delete buffer;
    return result;
}  

 
boost::python::list netSocket_recv	( netSocket & me, int MaxSize=20486, int flags = 0 )
{
    byte * buffer = new byte [MaxSize];
    int length,x;
    boost::python::list listout;
    length = me.recv ( (void *) buffer, MaxSize, flags);
    for (x=0;x<length;x++) {
        listout.append ( buffer[x] );
        }
    delete buffer;
    return  listout;
}
boost::python::str netSocket_recv_str	( netSocket & me, int MaxSize=2048, int flags = 0 )
{
    char * buffer = new char [MaxSize];
    int length;
    boost::python::str str_out;
    length = me.recv ( (void *) buffer, MaxSize, flags);
    if (length > 0 )
        str_out = boost::python::str (buffer, length);
    else
        str_out = boost::python::str();
    delete buffer;
    return  str_out;
}

boost::python::list netSocket_recvfrom ( netSocket & me, int MaxSize, int flags, netAddress* from ) 
{
    byte * buffer = new byte [MaxSize];
    int length,x;
    boost::python::list listout;
    length = me.recvfrom ( (void *) buffer, MaxSize, flags, from);
    for (x=0;x<length;x++) {
        listout.append ( buffer[x] );
        }
    delete buffer;
    return  listout;
}
boost::python::str netSocket_recvfrom_str ( netSocket & me, int MaxSize, int flags, netAddress* from ) 
{
    char * buffer = new char [MaxSize];
    int length;
    boost::python::str str_out;
    length = me.recvfrom ( (void *) buffer, MaxSize, flags, from);
    if (length > 0 ) 
        str_out = boost::python::str (buffer, length); 
    else
        str_out = boost::python::str();
    delete buffer;
    return  str_out;
}
"""
WRAPPER_REGISTRATION_netSocket = [
    """def( "send", &::netSocket_send,
    ( bp::arg("listin"), bp::arg("flags")=(0) ),
    "Python-Ogre Hand Wrapped\\n" );""",
    """def( "send", &::netSocket_send_str,
    ( bp::arg("listin"), bp::arg("flags")=(0) ),
    "Python-Ogre Hand Wrapped\\n" );""",
    """def( "sendto", &::netSocket_sendto,
    "Python-Ogre Hand Wrapped\\n" );""",
    """def( "sendto", &::netSocket_sendto_str,
    "Python-Ogre Hand Wrapped\\n" );""",
    
    """def( "recv", &::netSocket_recv,
    ( bp::arg("MaxSize")=(2048), bp::arg("flags")=(0) ),
    "Python-Ogre Hand Wrapped\\n" );""",
    """def( "recv_str", &::netSocket_recv_str,
    ( bp::arg("MaxSize")=(2048), bp::arg("flags")=(0) ),
    "Python-Ogre Hand Wrapped\\n" );""",
    """def( "recvfrom", &::netSocket_recvfrom,
    "Python-Ogre Hand Wrapped\\n" );""",
    """def( "recvfrom_str", &::netSocket_recvfrom_str,
    "Python-Ogre Hand Wrapped - receives a string\\n" );""",
]
    
    
WRAPPER_DEFINITION_netBuffer =\
"""
char * netBuffer_getData(netBuffer & me) { 
    return me.getData() ; 
    }
    
boost::python::str netBuffer_getData_str(netBuffer & me) { 
    boost::python::str str_out;
    int length = me.getLength();
    if (length > 0)
        str_out = boost::python::str( me.getData(), length );
    else
        str_out = boost::python::str();
    return str_out;
    }    
"""
WRAPPER_REGISTRATION_netBuffer = [
    """def( "getData", &::netBuffer_getData,
    bp::return_value_policy< bp::return_opaque_pointer >(),
    "Python-Ogre Hand Wrapped -  returns pointer to data buffer suitable to use with ctypes\\n" );""",
    """def( "getData_str", &::netBuffer_getData_str,
    "Python-Ogre Hand Wrapped - returns a string containing the buffer data\\n" );"""
    ]


WRAPPER_DEFINITION_netChannel =\
"""
int netChannel_send ( netChannel & me, boost::python::list listin, int flags = 0 )
{
    int index, result;
    int size = len(listin);
    byte * buffer = new byte [size];
    for (index=0;index<size;index++ ) {
        buffer[index] = (byte) boost::python::extract<int> (listin[index]);
    }
    result = me.send((const void *) buffer, size, flags );
    delete buffer;
    return result;
}

int netChannel_send_str ( netChannel & me, boost::python::str listin, int flags = 0 )
{
    int index, result;
    int size = len(listin);
    byte * buffer = new byte[size];
    for (index=0;index<size;index++ ) {
        buffer[index] = (byte) boost::python::extract<char> (listin[index]);
    }
    result = me.send((const void *) buffer, size, flags );
    delete buffer;
    return result;
}  
boost::python::list netChannel_recv	( netChannel & me, int MaxSize=2048, int flags = 0 )
{
    byte * buffer = new byte [MaxSize];
    int length,x;
    boost::python::list listout;
    length = me.recv ( (void *) buffer, MaxSize, flags);
    for (x=0;x<length;x++) {
        listout.append ( buffer[x] );
        }
    delete buffer;
    return  listout;
}
boost::python::str netChannel_recv_str	( netChannel & me, int MaxSize=2048, int flags = 0 )
{
    char * buffer = new char [MaxSize];
    int length;
    boost::python::str str_out;
    length = me.recv ( (void *) buffer, MaxSize, flags);
    if (length > 0 )
        str_out = boost::python::str (buffer, length);
    else
        str_out = boost::python::str();
    delete buffer;
    return  str_out;
}

"""
WRAPPER_REGISTRATION_netChannel = [
    """def( "send", &::netChannel_send,
    ( bp::arg("listin"), bp::arg("flags")=(0) ),
    "Python-Ogre Hand Wrapped\\n" );""",
    """def( "send", &::netChannel_send_str,
    ( bp::arg("listin"), bp::arg("flags")=(0) ),
    "Python-Ogre Hand Wrapped\\n" );""",
    """def( "recv", &::netChannel_recv,
    ( bp::arg("MaxSize")=(2048), bp::arg("flags")=(0) ),
    "Python-Ogre Hand Wrapped\\n" );""",
    """def( "recv_str", &::netChannel_recv_str,
    ( bp::arg("MaxSize")=(2048), bp::arg("flags")=(0) ),
    "Python-Ogre Hand Wrapped\\n" );""",
]

WRAPPER_DEFINITION_netMonitorServer =\
"""
  void setCommandFunc ( void (*func)(const char*) )
"""  


WRAPPER_DEFINITION_slScheduler =\
"""
int slScheduler_loopSample(slScheduler &me, slSample * s, int pri=0, 
                        slPreemptMode mode=::SL_SAMPLE_MUTE, int magic=0,
                        PyObject* subscriber=0, std::string const & method="")
{
    return me.loopSample ( s, pri, mode, magic );  // note calling with a null callback
}
int slScheduler_playSample(slScheduler &me, slSample * s, int pri=1, 
                        slPreemptMode mode=::SL_SAMPLE_ABORT, int magic=0,
                        PyObject* subscriber=0, std::string const & method="")
{
    return me.playSample ( s, pri, mode, magic );  // note calling with a null callback
}
int slScheduler_loopMusic(slScheduler &me, const char *fname, int pri=0, 
                        slPreemptMode mode=::SL_SAMPLE_MUTE, int magic=0,
                        PyObject* subscriber=0, std::string const & method="")
{
    return me.loopMusic ( fname, pri, mode, magic );  // note calling with a null callback
}
int slScheduler_playMusic(slScheduler &me, const char *fname, int pri=1, 
                        slPreemptMode mode=::SL_SAMPLE_ABORT, int magic=0,
                        PyObject* subscriber=0, std::string const & method="")
{
    return me.playMusic ( fname, pri, mode, magic );  // note calling with a null callback
}
"""                        

WRAPPER_REGISTRATION_slScheduler = [
    """def( "loopSample", &::slScheduler_loopSample,
    ( bp::arg("s"), bp::arg("pri")=(0), bp::arg("mode")=(::SL_SAMPLE_MUTE), 
                bp::arg("magic")=(0), bp::arg("subscriber")=bp::object(), bp::arg("method")="" ),
    "Python-Ogre Hand Wrapped\\n" );""",
    """def( "playSample", &::slScheduler_playSample,
    ( bp::arg("s"), bp::arg("pri")=(1), bp::arg("mode")=(::SL_SAMPLE_ABORT), 
                bp::arg("magic")=(0), bp::arg("subscriber")=bp::object(), bp::arg("method")="" ),
    "Python-Ogre Hand Wrapped\\n" );""",
    """def( "loopMusic", &::slScheduler_loopMusic,
    ( bp::arg("fname"), bp::arg("pri")=(0), bp::arg("mode")=(::SL_SAMPLE_MUTE), 
                bp::arg("magic")=(0), bp::arg("subscriber")=bp::object(), bp::arg("method")="" ),
    "Python-Ogre Hand Wrapped\\n" );""",
    """def( "playMusic", &::slScheduler_playMusic,
    ( bp::arg("fname"), bp::arg("pri")=(1), bp::arg("mode")=(::SL_SAMPLE_ABORT), 
                bp::arg("magic")=(0), bp::arg("subscriber")=bp::object(), bp::arg("method")="" ),
    "Python-Ogre Hand Wrapped\\n" );""",
   ] 
    
#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    rt = mb.class_( 'slScheduler' )
    rt.add_declaration_code( WRAPPER_DEFINITION_slScheduler )
    apply_reg (rt,  WRAPPER_REGISTRATION_slScheduler )

    rt = mb.class_( 'netSocket' )
    rt.add_declaration_code( WRAPPER_DEFINITION_netSocket )
    apply_reg (rt,  WRAPPER_REGISTRATION_netSocket )

    rt = mb.class_( 'netChannel' )
    rt.add_declaration_code( WRAPPER_DEFINITION_netChannel )
    apply_reg (rt,  WRAPPER_REGISTRATION_netChannel )

    rt = mb.class_( 'netBuffer' )
    rt.add_declaration_code( WRAPPER_DEFINITION_netBuffer )
    apply_reg (rt,  WRAPPER_REGISTRATION_netBuffer )
        
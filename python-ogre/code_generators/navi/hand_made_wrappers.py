import os
import environment

    
WRAPPER_DEFINITION_NaviManager = \
"""
class EventCallback
{
public:
    EventCallback(PyObject*  subscriber, std::string const & method)
    {
        mSubscriber = subscriber;
        mMethod = method;
    } 
    
    void call(const NaviLibrary::NaviData &naviData) 
    {
        if (mMethod.length() > 0 ) {
            boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                                    static_cast<const NaviLibrary::NaviData&>(naviData) );
        }
        else
            boost::python::call<void>(mSubscriber,  
                                    static_cast<const NaviLibrary::NaviData&>(naviData) );
    }
    PyObject*  mSubscriber;
    std::string  mMethod;
};


   
void NaviManager_bind(NaviLibrary::NaviManager &me, const std::string &naviName, 
                const std::string &naviDataName, 
                PyObject* subscriber, std::string const & method="",
                boost::python::list listin= boost::python::list())
{
    int index;
    
    // create a new call back function that knows how to call a python function or class method
    EventCallback * cb = new EventCallback( subscriber, method);
    
    // create the necessary Delegate wrapper around this new functions
    NaviLibrary::NaviDelegate callback = NaviLibrary::NaviDelegate ( cb, &EventCallback::call ); 
    
    // we allow a list of strings to be passed in and we convert these to a string vector 
    std::vector<std::string> myVector;
    for (index=0;index<len(listin);index++ )
        myVector.push_back ( boost::python::extract<std::string> (listin[index]) );
    
    // and finally call the bind function to make it all work
    me.bind(naviName, naviDataName, callback, myVector ) ; 
}

"""
WRAPPER_REGISTRATION_NaviManager = [
    """def( "bind", &::NaviManager_bind );"""
    
]

    
#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    rt = mb.class_( 'NaviManager' )
    rt.add_declaration_code( WRAPPER_DEFINITION_NaviManager )
    apply_reg (rt,  WRAPPER_REGISTRATION_NaviManager )
   
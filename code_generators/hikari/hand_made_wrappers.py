import os
import environment

WRAPPER_DEFINITION_FlashControl=\
"""
class PythonDispatcher
{
public:
	PythonDispatcher(PyObject*  subscriber, ::Ogre::String const & method) 
	{
        mSubscriber = subscriber;
        mMethod = method;
	}
	~PythonDispatcher() {}

    Hikari::FlashValue  callback ( Hikari::FlashControl* me, const Hikari::Arguments& args )   
    {
    Hikari::FlashValue retval ;
    if (mMethod.length() > 0 )
        retval = boost::python::call_method<Hikari::FlashValue>(mSubscriber, mMethod.c_str(), 
                            boost::ref(me), args );
    else
        retval = boost::python::call<Hikari::FlashValue>(mSubscriber, 
                           boost::ref(me), args );
    return retval;
    }

private:
	PyObject*  mSubscriber;
    std::string  mMethod;
};

// Bind function takes name of 'flash' command, plus a python object with optional method name if it's a class

std::map<std::string,PythonDispatcher *> m_mapDispatchers;

int FlashControl_bind(Hikari::FlashControl & me, std::string const & pFuncName, 
                        PyObject * subscriber, std::string const & method = "") 
{
	PythonDispatcher *pDispatch = new PythonDispatcher(subscriber, method);
	m_mapDispatchers[pFuncName] = pDispatch;
	me.bind(pFuncName, Hikari::FlashDelegate(pDispatch, &PythonDispatcher::callback));
	return 0;
}

int FlashControl_unbind(Hikari::FlashControl & me, std::string const & funcName)  
{
	me.unbind(funcName);
	std::map<std::string,PythonDispatcher *>::iterator it = m_mapDispatchers.find(funcName);
	if(it != m_mapDispatchers.end())
	{
		delete (*it).second;
		m_mapDispatchers.erase(it);
	}
	return 0;
}


"""
WRAPPER_REGISTRATION_FlashControl = [
    """def( "bind", &::FlashControl_bind ,
    "Python-Ogre Helper Function: Binds a python object as the callback for a flash event\\n\\
    Input: Flash Function/Event Name, Python Callback Option, Optional method name\\n\\
    Ouput: 0" );""",
    """def( "unbind", &::FlashControl_unbind ,
    "Python-Ogre Helper Function: Unbinds from a Flash event\\n\\
    Input: Flash Function/Event Name\\n\\
    Ouput: 0" );"""
    ] 
#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    rt = mb.class_( 'FlashControl' )
    rt.add_declaration_code( WRAPPER_DEFINITION_FlashControl )
    apply_reg (rt,  WRAPPER_REGISTRATION_FlashControl )
    
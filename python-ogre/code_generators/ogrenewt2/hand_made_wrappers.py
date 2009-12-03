import os
import environment

WRAPPER_DEFINITION_EventCallback = \
"""
//==========================================================
// Body_setCustomForceAndTorqueCallback Callback/Method:
//==========================================================
class EventCallback
{
public:
   PyObject*  mSubscriber;
    ::Ogre::String  mMethod;
//    std::cout << "**** IN CALL BACK C++ CODE";

    EventCallback(PyObject*  subscriber, ::Ogre::String const & method)
    {
        mSubscriber = subscriber;
        mMethod = method;
    } 
    
    ~EventCallback() {  } 
    
    void callback (OgreNewt::Body  *args) const
    {
    if (mMethod.length() > 0 )
        boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                            boost::ref(args) );
    else
        boost::python::call<void>(mSubscriber, 
                            boost::ref(args) );
    return;
    }

 
};

void Body_setCustomForceAndTorqueCallback(::OgreNewt::Body * self,
                                          PyObject* subscriber,
                                          ::Ogre::String const & method="")
{
    EventCallback * e = new EventCallback(subscriber, method);
    self->setCustomForceAndTorqueCallback( boost::bind( &EventCallback::callback, (EventCallback*)e, _1 ) );
};

//==========================================================
// Body_addBouyancyForce Callback/Method:
//==========================================================
class BouyancyCallback
{
public:
   PyObject*  mSubscriber;
    ::Ogre::String  mMethod;

    BouyancyCallback(PyObject*  subscriber, ::Ogre::String const & method)
    {
        mSubscriber = subscriber;
        mMethod = method;
    } 
    
    ~BouyancyCallback() { } 
    
    bool callback( int colID, OgreNewt::Body* me, const Ogre::Quaternion& orient, 
                                        const Ogre::Vector3& pos, Ogre::Plane& plane )
    {
    bool retval;
    if (mMethod.length() > 0 )
        retval = boost::python::call_method<bool>(mSubscriber, mMethod.c_str(), 
                            colID, boost::ref(me), boost::cref(orient), boost::cref(pos), boost::ref(plane) );
    else
        retval = boost::python::call<bool>(mSubscriber, 
                            colID, boost::ref(me), boost::cref(orient), boost::cref(pos), boost::ref(plane) );
    return retval;
    }

};

void Body_addBouyancyForce( ::OgreNewt::Body * self,
                            Ogre::Real fluidDensity,
                            Ogre::Real fluidLinearViscosity,
                            Ogre::Real fluidAngularViscosity,
                            const Ogre::Vector3& gravity,
                            PyObject* subscriber,
                            ::Ogre::String const & method="")
{
    BouyancyCallback * e = new BouyancyCallback(subscriber, method);
        
    self->addBouyancyForce( fluidDensity, fluidLinearViscosity, fluidAngularViscosity, gravity,
                            boost::bind(&BouyancyCallback::callback, (BouyancyCallback*)e, _1, _2, _3, _4, _5) );
};

"""
WRAPPER_REGISTRATION_EventCallback = [
    'def ("setCustomForceAndTorqueCallback", &::Body_setCustomForceAndTorqueCallback); ',
    'def ("addBouyancyForce", &::Body_addBouyancyForce); ',
    ]

#################################################################################################
#################################################################################################      

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    cs = mb.class_( 'Body' )
    cs.add_declaration_code( WRAPPER_DEFINITION_EventCallback )
    apply_reg (cs,  WRAPPER_REGISTRATION_EventCallback )


    

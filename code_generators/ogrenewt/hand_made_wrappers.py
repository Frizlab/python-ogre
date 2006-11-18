import os
import environment

WRAPPER_DEFINITION_Body=\
"""
void
Body_setUserData ( ::OgreNewt::Body & body, PyObject * data ) {
    body.setUserData ( data );
    }
    
PyObject *
Body_getUserData ( ::OgreNewt::Body & body) {
    void *  data = body.getUserData (  );
    Py_INCREF( (PyObject *) data );     // I'm passing a reference to this object so better inc the ref :)
    return  (PyObject *) data;
    }
 
boost::python::tuple
Body_getPositionOrientation ( ::OgreNewt::Body & body ) {
    Ogre::Vector3 pos;
    Ogre::Quaternion orient;
    body.getPositionOrientation  ( pos, orient );
    return boost::python::make_tuple(pos, orient);
    }
        
boost::python::tuple
Body_getMassMatrix ( ::OgreNewt::Body & body ) {
    Ogre::Real mass;
    Ogre::Vector3 inertia;
	body.getMassMatrix( mass,  inertia );
	return boost::python::make_tuple(mass, inertia);
	}
	
boost::python::tuple
Body_getInvMass ( ::OgreNewt::Body & body ) {
    Ogre::Real mass;
    Ogre::Vector3 inertia;
	body.getInvMass( mass,  inertia );
	return boost::python::make_tuple(mass, inertia);
	}
"""
WRAPPER_REGISTRATION_Body =\
"""
def ("setUserData", &::Body_setUserData );
Body_exposer.def ("getUserData", &::Body_getUserData);
Body_exposer.def ("getPositionOrientation", &::Body_getPositionOrientation);
Body_exposer.def ("getMassMatrix", &::Body_getMassMatrix);
Body_exposer.def ("getInvMass", &::Body_getInvMass);
"""

WRAPPER_DEFINITION_Joint=\
"""
void
Joint_setUserData ( ::OgreNewt::Joint & joint, PyObject * data ) {
    joint.setUserData ( data );
    }
PyObject *
Joint_getUserData ( ::OgreNewt::Joint & joint) {
    void *  data = joint.getUserData (  );
    Py_INCREF( (PyObject *) data );     // I'm passing a reference to this object so better inc the ref :)
    return  (PyObject *) data;
    }
"""

WRAPPER_REGISTRATION_Joint =\
"""
def ("setUserData", &::Joint_setUserData );
Joint_exposer.def ("getUserData", &::Joint_getUserData);
"""

WRAPPER_DEFINITION_EventCallback = \
"""
class EventCallback
{
public:
   PyObject*  mSubscriber;
    ::Ogre::String  mMethod;

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
                            args );
    else
        boost::python::call<void>(mSubscriber, 
                            args );
    return;
    }

 
};

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
                            colID, me, orient, pos, plane );
    else
        retval = boost::python::call<bool>(mSubscriber, 
                            colID, me, orient, pos, plane );
    return retval;
    }

};

class AutoactiveCallback
{
public:
   PyObject*  mSubscriber;
    ::Ogre::String  mMethod;

    AutoactiveCallback(PyObject*  subscriber, ::Ogre::String const & method)
    {
        mSubscriber = subscriber;
        mMethod = method;
    } 
    
    ~AutoactiveCallback() { } 
    
    void callback( OgreNewt::Body* body, unsigned int id )
    {
    if (mMethod.length() > 0 )
        boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                            body, id );
    else
        boost::python::call<void>(mSubscriber, 
                            body, id );
    return;
    }

};

class CustomTransformCallback
{
public:
   PyObject*  mSubscriber;
    ::Ogre::String  mMethod;

    CustomTransformCallback(PyObject*  subscriber, ::Ogre::String const & method)
    {
        mSubscriber = subscriber;
        mMethod = method;
    } 
    
    ~CustomTransformCallback() { } 
    
    void callback( OgreNewt::Body* body, const Ogre::Quaternion& orient , const Ogre::Vector3&  vect)
    {
    if (mMethod.length() > 0 )
        boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                            body, orient, vect );
    else
        boost::python::call<void>(mSubscriber, 
                            body, orient, vect );
    return;
    }

};

void Body_setCustomForceAndTorqueCallback( ::OgreNewt::Body * self, PyObject* subscriber, ::Ogre::String const & method="")
{
    EventCallback * e = new EventCallback(subscriber, method);
    self->setCustomForceAndTorqueCallback<EventCallback>( &EventCallback::callback, (EventCallback*) e);
};


void Body_addBouyancyForce( ::OgreNewt::Body * self, Ogre::Real fluidDensity, Ogre::Real fluidLinearViscosity , 
                    Ogre::Real fluidAngularViscosity , const Ogre::Vector3& gravity, PyObject* subscriber, ::Ogre::String const & method="")
{
    BouyancyCallback * e = new BouyancyCallback(subscriber, method);
    self->addBouyancyForce<BouyancyCallback>(fluidDensity, fluidLinearViscosity, fluidAngularViscosity,
                            gravity, &BouyancyCallback::callback, (BouyancyCallback*) e);
};

void Body_setAutoactiveCallback( ::OgreNewt::Body * self, PyObject* subscriber, ::Ogre::String const & method="" )
{
    AutoactiveCallback * e = new AutoactiveCallback(subscriber, method);
    self->setAutoactiveCallback<AutoactiveCallback>(&AutoactiveCallback::callback, (AutoactiveCallback*) e);
};

void Body_setCustomTransformCallback( ::OgreNewt::Body * self, PyObject* subscriber, ::Ogre::String const & method="" )
{
    CustomTransformCallback * e = new CustomTransformCallback(subscriber, method);
    self->setCustomTransformCallback<CustomTransformCallback>(&CustomTransformCallback::callback, (CustomTransformCallback*) e);
};
"""

WRAPPER_REGISTRATION_EventCallback =\
"""

def ("setCustomForceAndTorqueCallback", &::Body_setCustomForceAndTorqueCallback);
Body_exposer.def ("addBouyancyForce", &::Body_addBouyancyForce);
Body_exposer.def ("setAutoactiveCallback", &::Body_setAutoactiveCallback);
Body_exposer.def ("setCustomTransformCallback", &::Body_setCustomTransformCallback);


"""



def apply( mb ):
    cs = mb.class_( 'Body' )
    cs.add_declaration_code( WRAPPER_DEFINITION_Body )
    cs.add_declaration_code( WRAPPER_DEFINITION_EventCallback )
    cs.add_registration_code( WRAPPER_REGISTRATION_Body )
    cs.add_registration_code( WRAPPER_REGISTRATION_EventCallback )
    

    cs = mb.class_( 'Joint' )
    cs.add_declaration_code( WRAPPER_DEFINITION_Joint )
    cs.add_registration_code( WRAPPER_REGISTRATION_Joint )
        
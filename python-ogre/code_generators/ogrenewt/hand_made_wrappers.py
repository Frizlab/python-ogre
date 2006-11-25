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
	
boost::python::tuple
Body_getFreezeThreshold ( ::OgreNewt::Body & body ) {
    Ogre::Real speed;
    Ogre::Real omega;
	body.getFreezeThreshold( speed,  omega );
	return boost::python::make_tuple(speed, omega);
	}
	
"""

WRAPPER_REGISTRATION_Body =\
"""
def ("setUserData", &::Body_setUserData );
Body_exposer.def ("getUserData", &::Body_getUserData);
Body_exposer.def ("getPositionOrientation", &::Body_getPositionOrientation);
Body_exposer.def ("getMassMatrix", &::Body_getMassMatrix);
Body_exposer.def ("getInvMass", &::Body_getInvMass);
Body_exposer.def ("getFreezeThreshold", &::Body_getFreezeThreshold);
"""

WRAPPER_DEFINITION_ConvexCollision=\
"""
//NEWTON_API void NewtonConvexCollisionCalculateInertialMatrix (const NewtonCollision* convexCollision, dFloat* inertia, dFloat* origin);	

boost::python::tuple
ConvexCollision_calculateInertialMatrix ( ::OgreNewt::ConvexCollision & convexC ) {
    Ogre::Vector3 inertia;
    Ogre::Vector3 offset;
    convexC.calculateInertialMatrix  ( inertia,  offset );
    return boost::python::make_tuple(inertia, offset);
    }
"""
WRAPPER_REGISTRATION_ConvexCollision =\
"""
def ("calculateInertialMatrix", &::ConvexCollision_calculateInertialMatrix );
"""


WRAPPER_DEFINITION_CustomJoint=\
"""
boost::python::tuple
CustomJoint_pinAndDirToLocal ( ::OgreNewt::CustomJoint & joint, const  Ogre::Vector3 pinpt, const Ogre::Vector3 pindir) {
    Ogre::Quaternion localOrient0;
    Ogre::Vector3 localPos0;
    Ogre::Quaternion localOrient1;
    Ogre::Vector3 localPos1;
    joint.pinAndDirToLocal( pinpt,  pindir, 
                        localOrient0, localPos0, 
                        localOrient1, localPos1 );
    return boost::python::make_tuple(localOrient0, localPos0, localOrient1, localPos1);
    }
    
boost::python::tuple
CustomJoint_localToGlobal ( ::OgreNewt::CustomJoint & joint, const Ogre::Quaternion& localOrient, const Ogre::Vector3& localPos, int bodyIndex) {
    Ogre::Quaternion globalOrient;
    Ogre::Vector3 globalPos;
    joint.localToGlobal(localOrient, localPos, 
                        globalOrient, globalPos,
                         bodyIndex);
    return boost::python::make_tuple(globalOrient, globalPos);
    }    
"""
WRAPPER_REGISTRATION_CustomJoint =\
"""
def ("pinAndDirToLocal", &::CustomJoint_pinAndDirToLocal );
def ("localToGlobal", &::CustomJoint_localToGlobal );
"""






WRAPPER_DEFINITION_VehicleTire=\
"""
boost::python::tuple
Vehicle_getPositionOrientation ( ::OgreNewt::Vehicle::Tire & tire ) {
    Ogre::Quaternion orient;
    Ogre::Vector3 pos;
    tire.getPositionOrientation ( orient, pos );
    return boost::python::make_tuple(orient, pos);
    }
"""

WRAPPER_REGISTRATION_VehicleTire =\
"""
def ("getPositionOrientation", &::Vehicle_getPositionOrientation );
"""

## more functions that need to modifiy the attributes passed to them
WRAPPER_DEFINITION_ContactCallback=\
"""
//	void getContactPositionAndNormal( Ogre::Vector3& pos, Ogre::Vector3& norm ) const;
//	void getContactTangentDirections( Ogre::Vector3& dir0, Ogre::Vector3& dir1 ) const;
boost::python::tuple
ContactCallback_getContactPositionAndNormal ( ::OgreNewt::ContactCallback & CC ) {
    Ogre::Vector3 pos;
    Ogre::Vector3 norm;
    CC.getContactPositionAndNormal ( pos, norm );
    return boost::python::make_tuple(pos, norm);
    }
boost::python::tuple
ContactCallback_getContactTangentDirections ( ::OgreNewt::ContactCallback & CC ) {
    Ogre::Vector3 dir0;
    Ogre::Vector3 dir1;
    CC.getContactTangentDirections  ( dir0, dir1 );
    return boost::python::make_tuple(dir0, dir1);
    }    
"""
WRAPPER_REGISTRATION_ContactCallback =\
"""
def ("getContactPositionAndNormal", &::ContactCallback_getContactPositionAndNormal );
ContactCallback_exposer.def ("getContactTangentDirections", &::ContactCallback_getContactTangentDirections );
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
                            colID, boost::ref(me), boost::cref(orient), boost::cref(pos), boost::python::ptr(plane) );
    else
        retval = boost::python::call<bool>(mSubscriber, 
                            colID, boost::ref(me), boost::cref(orient), boost::cref(pos), boost::python::ptr(plane) );
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
                            boost::ref(body), id );
    else
        boost::python::call<void>(mSubscriber, 
                           boost::ref(body), id );
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
                            boost::ref(body), boost::cref(orient), boost::cref(vect) );
    else
        boost::python::call<void>(mSubscriber, 
                            boost::ref(body), boost::cref(orient), boost::cref(vect) );
    return;
    }

};

void Body_setCustomForceAndTorqueCallback( ::OgreNewt::Body * self, PyObject* subscriber, ::Ogre::String const & method="")
{
    EventCallback * e = new EventCallback(subscriber, method);
    self->setCustomForceAndTorqueCallback<EventCallback>( &EventCallback::callback, (EventCallback*) e);
//    return e;
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
//           bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
Body_exposer.def ("addBouyancyForce", &::Body_addBouyancyForce);
Body_exposer.def ("setAutoactiveCallback", &::Body_setAutoactiveCallback);
Body_exposer.def ("setCustomTransformCallback", &::Body_setCustomTransformCallback);
/*
//{ //::OgreNewt::EventCallback
//        typedef bp::class_< ::EventCallback, boost::noncopyable > EventCallback_exposer_t;
//        EventCallback_exposer_t EventCallback_exposer = EventCallback_exposer_t( "EventCallback", bp::init< PyObject *, Ogre::String const & >(( bp::arg("subscriber"), bp::arg("method") )) );
//        bp::scope EventCallback_scope( EventCallback_exposer );
//        { //::OgreNewt::EventCallback::callback
        
//            typedef void ( ::EventCallback::*callback_function_type )( ::OgreNewt::Body * ) const;
            
//            EventCallback_exposer.def( 
//                "callback"
//                , callback_function_type( &::EventCallback::callback )
//                , ( bp::arg("args") ) );
//        
//        }
//    }

*/

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
    
    cs = mb.class_( 'ConvexCollision' )
    cs.add_declaration_code( WRAPPER_DEFINITION_ConvexCollision )
    cs.add_registration_code( WRAPPER_REGISTRATION_ConvexCollision )

    cs = mb.class_( 'ContactCallback' )
    cs.add_declaration_code( WRAPPER_DEFINITION_ContactCallback )
    cs.add_registration_code( WRAPPER_REGISTRATION_ContactCallback )
    
    cs = mb.class_( 'Vehicle' ).class_( 'Tire' )
    cs.add_declaration_code( WRAPPER_DEFINITION_VehicleTire )
    cs.add_registration_code( WRAPPER_REGISTRATION_VehicleTire )
    
    cs = mb.class_( 'CustomJoint' )
    cs.add_declaration_code( WRAPPER_DEFINITION_CustomJoint )
    cs.add_registration_code( WRAPPER_REGISTRATION_CustomJoint )
                
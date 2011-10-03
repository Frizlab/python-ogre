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
"""


WRAPPER_REGISTRATION_Body =[
    'def ("setUserData", &::Body_setUserData );',
    'def ("getUserData", &::Body_getUserData);'
    ]
    

#################################################################################

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

WRAPPER_REGISTRATION_Joint = [
    'def ("setUserData", &::Joint_setUserData );',
    'def ("getUserData", &::Joint_getUserData);'
    ]

#################################################################################

WRAPPER_DEFINITION_Hinge=\
"""

class HingeCallback
{
public:
    PyObject*  cb_object;
    ::Ogre::String  cb_method;
    
    HingeCallback( PyObject*  subscriber, ::Ogre::String const & method)
    {
        cb_object = subscriber;
        cb_method = method;
    } 
    
    ~HingeCallback() { } 
    
    void callback( OgreNewt::BasicJoints::Hinge * me)
    {
        if (cb_method.length() > 0 )
            boost::python::call_method<void>(cb_object, cb_method.c_str(), 
                                boost::ref(me) );
        else
            boost::python::call<void>(cb_object, 
                                boost::ref(me) );
    }

};
HingeCallback * Hcb;
static void StaticCallBack( OgreNewt::BasicJoints::Hinge * me)
{
    Hcb->callback ( me );
}


/* here is the routine that gets called from Python to setup the callback.. It creates a new class
that handles the fact that the python function can be a class method or a stand alone function.
However the call back that gets passed to the OgreNewt library MUST be a STATIC function - this is to ensure
that the calling conventions being used are 'C' ones, not C++...  
This means that there will only every be ONE PYTHON function that receives all the callbacks for Hinges..
So if you really want it to do different things then you will need the python function to look at the hinge
it's been passed and manage the callback appropiately
*/
void Hinge_setHingeCallback( ::OgreNewt::BasicJoints::Hinge * me, 
                                            PyObject* subscriber, ::Ogre::String const & method="")
{
    Hcb = new HingeCallback(subscriber, method);
    me->setCallback( ( ::OgreNewt::BasicJoints::Hinge::HingeCallback) &StaticCallBack );
};
"""
WRAPPER_REGISTRATION_Hinge = [
    'def ("setCallback", &::Hinge_setHingeCallback); '
    ]
    
#################################################################################

WRAPPER_DEFINITION_Slider=\
"""

class SliderCallback
{
public:
   PyObject*  mSubscriber;
    ::Ogre::String  mMethod;

    SliderCallback(PyObject*  subscriber, ::Ogre::String const & method)
    {
        mSubscriber = subscriber;
        mMethod = method;
    } 
    
    ~SliderCallback() { } 
    
    void callback ( OgreNewt::BasicJoints::Slider * me)
    {
    if (mMethod.length() > 0 )
        boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                            boost::ref(me) );
    else
        boost::python::call<void>(mSubscriber, 
                            boost::ref(me) );
    return;
    }

};

SliderCallback * Scb;
static void StaticCallBack( OgreNewt::BasicJoints::Slider * me)
{
    Scb->callback ( me );
}


void Slider_setSliderCallback( ::OgreNewt::BasicJoints::Slider * me, 
                                            PyObject* subscriber, ::Ogre::String const & method="")
{
    Scb = new SliderCallback(subscriber, method);
    me->setCallback( ( ::OgreNewt::BasicJoints::Slider::SliderCallback ) &StaticCallBack);
};
"""
WRAPPER_REGISTRATION_Slider = [
    'def ("setCallback", &::Slider_setSliderCallback); '
    ]
        
############################################################################################

WRAPPER_DEFINITION_Universal=\
"""

class UniversalCallback
{
public:
   PyObject*  mSubscriber;
    ::Ogre::String  mMethod;

    UniversalCallback(PyObject*  subscriber, ::Ogre::String const & method)
    {
        mSubscriber = subscriber;
        mMethod = method;
    } 
    
    ~UniversalCallback() { } 
    
    void callback ( OgreNewt::BasicJoints::Universal * me)
    {
    if (mMethod.length() > 0 )
        boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                            boost::ref(me) );
    else
        boost::python::call<void>(mSubscriber, 
                            boost::ref(me) );
    return;
    }

};

UniversalCallback * Ucb;
static void StaticCallBack( OgreNewt::BasicJoints::Universal * me)
{
    Ucb->callback ( me );
}

void Universal_setUniversalCallback( ::OgreNewt::BasicJoints::Universal * me, 
                                                PyObject* subscriber, ::Ogre::String const & method="")
{
    Ucb = new UniversalCallback(subscriber, method);
    me->setCallback( ( ::OgreNewt::BasicJoints::Universal::UniversalCallback ) &StaticCallBack);
};
"""
WRAPPER_REGISTRATION_Universal = [
    'def ("setCallback", &::Universal_setUniversalCallback); '
    ]

    
############################################################################################

WRAPPER_DEFINITION_BodyIterator=\
"""

class BodyIteratorCallback
{
public:
   PyObject*  mSubscriber;
    ::Ogre::String  mMethod;

    BodyIteratorCallback(PyObject*  subscriber, ::Ogre::String const & method)
    {
        mSubscriber = subscriber;
        mMethod = method;
    } 
    
    ~BodyIteratorCallback() { } 
    
    void operator() ( OgreNewt::BodyIterator * me)
    {
    if (mMethod.length() > 0 )
        boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                            boost::ref(me) );
    else
        boost::python::call<void>(mSubscriber, 
                            boost::ref(me) );
    return;
    }

};

void BodyIterator_setBodyIteratorCallback( ::OgreNewt::BodyIterator * me, 
                                                PyObject* subscriber, ::Ogre::String const & method="")
{
    BodyIteratorCallback * cb = new BodyIteratorCallback(subscriber, method);
    me->go( ( ::OgreNewt::BodyIterator::IteratorCallback ) cb);
};
"""
WRAPPER_REGISTRATION_BodyIterator = [
    'def ("go", &::BodyIterator_setBodyIteratorCallback); '
    ]            
############################################################################################



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
                            colID, boost::ref(me), boost::cref(orient), boost::cref(pos), boost::ref(plane) );
    else
        retval = boost::python::call<bool>(mSubscriber, 
                            colID, boost::ref(me), boost::cref(orient), boost::cref(pos), boost::ref(plane) );
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

void Body_setCustomForceAndTorqueCallback( ::OgreNewt::Body * body, PyObject* subscriber, ::Ogre::String const & method="")
{
    EventCallback * e = new EventCallback(subscriber, method);
    body->setCustomForceAndTorqueCallback<EventCallback>( &EventCallback::callback, (EventCallback*) e);
};


void Body_addBouyancyForce( ::OgreNewt::Body * self, Ogre::Real fluidDensity, Ogre::Real fluidLinearViscosity , 
                    Ogre::Real fluidAngularViscosity , const Ogre::Vector3& gravity, 
                                                            PyObject* subscriber, ::Ogre::String const & method="")
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
    self->setCustomTransformCallback<CustomTransformCallback>(&CustomTransformCallback::callback, 
                                                                                    (CustomTransformCallback*) e);
};
"""



WRAPPER_REGISTRATION_EventCallback = [
    'def ("setCustomForceAndTorqueCallback", &::Body_setCustomForceAndTorqueCallback); ',
    'def ("addBouyancyForce", &::Body_addBouyancyForce);',
    'def ("setAutoactiveCallback", &::Body_setAutoactiveCallback);',
    'def ("setCustomTransformCallback", &::Body_setCustomTransformCallback);'
    ]
    
    
    
WRAPPER_DEFINITION_TreeCollision=\
"""
void
TreeCollision_addPoly( ::OgreNewt::CollisionPrimitives::TreeCollision & me, boost::python::list polys, unsigned int ID )
   {
    
	Ogre::Vector3* newverts = new Ogre::Vector3[ 3 ];
	Ogre::Vector3* startpos = newverts;
    int index;
    if (len(polys) < 3 ) {
        throw std::runtime_error ( std::string ("addPoly called with a list with less than 3 verticies") );
        return;
        }
    for (index=0;index<3;index++ ) {
        *newverts++ = boost::python::extract<Ogre::Vector3> (polys[index]);
        }
	me.addPoly( startpos, ID );
    return;
	}
	"""
WRAPPER_REGISTRATION_TreeCollision= [
    """def ("addPoly", &::TreeCollision_addPoly,\
                "Python-Ogre Helper Function: Adds a list of 3 Ogre:Vector3's to the collision system.\\n\\
                Input: List of 3 Ogre::Vector3's, ID\\n\\
                Output: Nothing"  );"""
    ]
    
        
########################################################################################
WRAPPER_DEFINITION_General=\
"""
OgreNewt::ConvexCollision *
//OgreNewt::CollisionPrimitives::ConvexHull *
General_createConvexHull( const OgreNewt::World* world, boost::python::list verts, int vertcount )
//				const Ogre::Quaternion& orient = Ogre::Quaternion::IDENTITY, const Ogre::Vector3& pos = Ogre::Vector3::ZERO )

   {
	Ogre::Vector3* newverts = new Ogre::Vector3[ len(verts) ];
	Ogre::Vector3* startpos = newverts;
    int index;

    for (index=0;index<len(verts);index++ ) {
        *newverts++ = boost::python::extract<Ogre::Vector3> (verts[index]);
        }
	OgreNewt::ConvexCollision* col = new OgreNewt::CollisionPrimitives::ConvexHull( world, startpos, len(verts) );
    //return &OgreNewt::CollisionPrimitives::ConvexHull( world, startpos, len(verts) );
	return col;
	}
	"""

WRAPPER_REGISTRATION_General= [
    'def ("createConvexHull", &::General_createConvexHull,\
     bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());'
    ]
    
    
    
def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )

def apply( mb ):
    cs = mb.class_( 'Body' )
    cs.add_declaration_code( WRAPPER_DEFINITION_Body )
    cs.add_declaration_code( WRAPPER_DEFINITION_EventCallback )
    apply_reg (cs,  WRAPPER_REGISTRATION_EventCallback )
    apply_reg (cs,  WRAPPER_REGISTRATION_Body )

 
    cs = mb.class_( 'Joint' )
    cs.add_declaration_code( WRAPPER_DEFINITION_Joint )
    apply_reg (cs,  WRAPPER_REGISTRATION_Joint )
    
    cs = mb.namespace( 'BasicJoints' ).class_("Hinge" )
    cs.add_declaration_code( WRAPPER_DEFINITION_Hinge )
    apply_reg (cs,  WRAPPER_REGISTRATION_Hinge )
    cs = mb.namespace( 'BasicJoints' ).class_("Slider" )
    cs.add_declaration_code( WRAPPER_DEFINITION_Slider )
    apply_reg (cs,  WRAPPER_REGISTRATION_Slider )
    
    cs = mb.namespace( 'BasicJoints' ).class_("Universal" )
    cs.add_declaration_code( WRAPPER_DEFINITION_Universal )
    apply_reg (cs,  WRAPPER_REGISTRATION_Universal )
    
    cs = mb.class_( 'BodyIterator' )
    cs.add_declaration_code( WRAPPER_DEFINITION_BodyIterator )
    apply_reg (cs,  WRAPPER_REGISTRATION_BodyIterator )
    
    cs = mb.class_( 'TreeCollision' )
    cs.add_declaration_code( WRAPPER_DEFINITION_TreeCollision )
    apply_reg (cs,  WRAPPER_REGISTRATION_TreeCollision )

    mb.add_declaration_code( WRAPPER_DEFINITION_General )
    apply_reg (mb,  WRAPPER_REGISTRATION_General )
      
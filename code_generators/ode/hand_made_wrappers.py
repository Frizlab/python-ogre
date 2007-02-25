import os
import environment

WRAPPER_DEFINITION_dBody=\
"""
void
dBody_setData ( ::dBody & me, PyObject * data ) {
    me.setData ( data );
    }
    
PyObject *
dBody_getData ( ::dBody & me ) {
    void *  data = me.getData (  );
    Py_INCREF( (PyObject *) data );     // I'm passing a reference to this object so better inc the ref :)
    return  (PyObject *) data;
    }        
"""        

WRAPPER_REGISTRATION_dBody = \
"""
    dBody_exposer.def( "getData", &::dBody_getData);
    dBody_exposer.def( "setData", &::dBody_setData);
"""

#######################################################
#######################################################

WRAPPER_DEFINITION_dGeom=\
"""

void
dGeom_setData ( ::dGeom & me, PyObject * data ) {
    me.setData ( data );
    }
    
PyObject *
dGeom_getData ( ::dGeom & me ) {
    void *  data = me.getData (  );
    Py_INCREF( (PyObject *) data );     // I'm passing a reference to this object so better inc the ref :)
    return  (PyObject *) data;
    }               
"""


WRAPPER_REGISTRATION_dGeom = \
"""
    dGeom_exposer.def ("setData", &::dGeom_setData );
    dGeom_exposer.def ("getData", &::dGeom_getData);
"""


#################################################################################

WRAPPER_DEFINITION_dSpace = \
"""
class nearCallback
{
public:
   PyObject*  CallBackObject;
   std::string  CallBackFunction;
   boost::python::object CallBackData;
    // we store the data etc here
    nearCallback(boost::python::object data, PyObject*  cb_object, std::string const & cb_function )
    {
        CallBackObject = cb_object;
        CallBackFunction = cb_function;
        CallBackData = data;
    } 
    
    
   // static call back function needed here as it matches the call signature needed by the ODE library
   
   static void callback (void * data, dGeomID o1, dGeomID o2) 
    {
    nearCallback * ActualClass = (nearCallback * )  data; // get a pointer to the instanced class
    // dGeomID's are opaque pointers so I'll do a little work before passing them on..
//    dBodyID b1 = dGeomGetBody(o1);
//    dBodyID b2 = dGeomGetBody(o2);
    
    if (ActualClass->CallBackFunction.length() > 0 )
        boost::python::call_method<void>(ActualClass->CallBackObject, ActualClass->CallBackFunction.c_str(), 
                            ActualClass->CallBackData, 
                           (void *)o1, (void *)o2 );
    else
        boost::python::call<void>(ActualClass->CallBackObject, 
                            ActualClass->CallBackData,
                           (void *)o1, (void *)o2 );
    return;
    }

 
};
void dSpace_collide( ::dSpace * self,  boost::python::object data, PyObject* subscriber, std::string const & method="")
{
    // store the 'data' in the class as well
    nearCallback * e = new nearCallback(data, subscriber, method);
    // then use the 'data' field to store point to the static call back function
    self->collide( (void*) e, &nearCallback::callback);
};

"""

WRAPPER_REGISTRATION_dSpace =\
"""
def ("collide", &::dSpace_collide); 

"""

WRAPPER_DEFINITION_General = \
"""
dSimpleSpace *
General_CreateSimpleSpace (void) {
    return new dSimpleSpace( 0 );
}

dHashSpace *
General_CreateHashSpace (void ) {
    return new dHashSpace( 0 );
}

"""

WRAPPER_REGISTRATION_General =\
"""
    bp::def ("CreateSimpleSpace", &::General_CreateSimpleSpace,
        bp::return_value_policy< bp::reference_existing_object>());
    bp::def ("CreateHashSpace", &::General_CreateHashSpace,
        bp::return_value_policy< bp::reference_existing_object>());

"""
#################################################################################
#################################################################################




def apply( mb ):
#     pass
    cs = mb.class_( 'dBody' )
    cs.add_declaration_code( WRAPPER_DEFINITION_dBody )
    cs.add_registration_code( WRAPPER_REGISTRATION_dBody )
    cs = mb.class_( 'dGeom' )
    cs.add_declaration_code( WRAPPER_DEFINITION_dGeom )
    cs.add_registration_code( WRAPPER_REGISTRATION_dGeom )
    cs = mb.class_( 'dSpace' )
    cs.add_declaration_code( WRAPPER_DEFINITION_dSpace )
    cs.add_registration_code( WRAPPER_REGISTRATION_dSpace )
    
    mb.add_declaration_code( WRAPPER_DEFINITION_General )
    mb.add_registration_code( WRAPPER_REGISTRATION_General )
    
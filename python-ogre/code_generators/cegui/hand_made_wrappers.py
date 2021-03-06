import os
import environment

WRAPPER_DEFINITION_String =\
"""
// Helper function to pass a python string to a CEGUI::String
void StringAssign(CEGUI::String &me, PyObject * stringin)
{
    if (PyUnicode_Check( stringin )) { // OK so it's a unicodeobject
    	me.assign ( (CEGUI::utf8 * ) PyUnicode_AS_DATA ( stringin ) );
    	}
    else if (PyString_Check( stringin )) { // OK so it's a unicodeobject
    	me.assign ( (CEGUI::utf8 * ) PyString_AsString ( stringin ) );
    	}
    return ;
    }
"""
WRAPPER_REGISTRATION_String =\
"""
def ( "assign",&StringAssign);
"""

WRAPPER_REGISTRATION_EventSet = \
"""  
def( "subscribeEvent", &EventSet_subscribeEventPB, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventTB, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventLB, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventComboBox, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventCheckBox, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventSB, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventGUISheet, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventFW, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventEB, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventThumb, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventSlider, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventSpinner, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventRadioButton, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeDragContainer, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeMultiColumnList, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeWindow, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeMouseCursor, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeSystem, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventSet, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
#if CEGUI_VERSION_MINOR > 5 && CEGUI_VERSION_PATCH > 0
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeTree, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
#endif
#if CEGUI_VERSION_MINOR < 7 
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeRenderer, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
#endif
                        
{ //EventConnection
        typedef bp::class_< EventConnection > EventConnection_exposer_t;
        EventConnection_exposer_t EventConnection_exposer = EventConnection_exposer_t( "EventConnection" );
        bp::scope EventConnection_scope( EventConnection_exposer );
        EventConnection_exposer.def( bp::init< >() );
        { //::CEGUI::EventConnection::connected
        
            typedef bool ( EventConnection::*connected_function_type )(  ) ;
            
            EventConnection_exposer.def( 
                "connected"
                , connected_function_type( &EventConnection::connected ) );
        
        }
        { //::CEGUI::EventConnection::disconnect
        
            typedef void ( EventConnection::*disconnect_function_type )(  ) ;
            
            EventConnection_exposer.def( 
                "disconnect"
                , disconnect_function_type( &EventConnection::disconnect ) );
        
        }
    }
"""

WRAPPER_DEFINITION_EventSet = \
"""
class EventCallback
{
public:
    EventCallback() : mSubscriber(0) {}

    EventCallback(PyObject*  subscriber, CEGUI::String const & method)
    {
        mSubscriber = subscriber;
        mMethod = method;
    } 
    
    EventCallback(const EventCallback &other)
    : mSubscriber(0)
    {
        *this = other;
    }
    
    ~EventCallback()
    {
    } 
    
    void setsubscriber( PyObject* subscriber )
    {
        mSubscriber = subscriber;
    }
    
  bool operator() (const CEGUI::EventArgs &args) const
    {
        if (dynamic_cast<CEGUI::MouseCursorEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                                        static_cast<const CEGUI::MouseCursorEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber, 
                                        static_cast<const CEGUI::MouseCursorEventArgs&>(args) );
            
        else if (dynamic_cast<CEGUI::KeyEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                                        static_cast<const CEGUI::KeyEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber, 
                                        static_cast<const CEGUI::KeyEventArgs&>(args) );
        
        else if (dynamic_cast<CEGUI::ActivationEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                                        static_cast<const CEGUI::ActivationEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber,  
                                        static_cast<const CEGUI::ActivationEventArgs&>(args) );
        
        else if (dynamic_cast<CEGUI::DragDropEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                                        static_cast<const CEGUI::DragDropEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber,  
                                        static_cast<const CEGUI::DragDropEventArgs&>(args) );
        
        else if (dynamic_cast<CEGUI::HeaderSequenceEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                                        static_cast<const CEGUI::HeaderSequenceEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber,
                                        static_cast<const CEGUI::HeaderSequenceEventArgs&>(args) );
        
        else if (dynamic_cast<CEGUI::MouseEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                                        static_cast<const CEGUI::MouseEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber, 
                                        static_cast<const CEGUI::MouseEventArgs&>(args) );

        else if (dynamic_cast<CEGUI::DisplayEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(),
                                        static_cast<const CEGUI::DisplayEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber,
                                        static_cast<const CEGUI::DisplayEventArgs&>(args) );

        else if (dynamic_cast<CEGUI::ResourceEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(),
                                        static_cast<const CEGUI::ResourceEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber,
                                        static_cast<const CEGUI::ResourceEventArgs&>(args) );

         else if (dynamic_cast<CEGUI::TreeEventArgs *>((CEGUI::EventArgs *)&args))
           if (mMethod.length() > 0 )
               boost::python::call_method<void>(mSubscriber, mMethod.c_str(),
                                        static_cast<const CEGUI::TreeEventArgs&>(args) );
           else
               boost::python::call<void>(mSubscriber, 
                                        static_cast<const CEGUI::TreeEventArgs&>(args) );

        else if (dynamic_cast<CEGUI::WindowEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                                        static_cast<const CEGUI::WindowEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber,  
                                        static_cast<const CEGUI::WindowEventArgs&>(args) );

        else 
            boost::python::call_method<void>(mSubscriber, mMethod.c_str(), args );
     return true;
    }

    PyObject*  mSubscriber;
    CEGUI::String  mMethod;
};

class EventConnection
{
public:
    EventConnection() : mConnection(0), mValid(false) {}
    
    EventConnection(const CEGUI::Event::Connection &connection) 
    : mConnection(connection), mValid(true) 
    {
    }
    ~EventConnection()
    {
        if (mCallback) delete mCallback;
    }   
    bool connected()
    {
        return mValid && mConnection->connected();
    }
    
    void disconnect()
    {
        if (mValid)
            mConnection->disconnect();
    } // disconnect()
    
protected:
    CEGUI::Event::Connection mConnection;
    bool mValid;
    EventCallback * mCallback;
};



EventConnection * EventSet_subscribeEventSet(CEGUI::EventSet *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}

EventConnection * EventSet_subscribeEventPB(CEGUI::PushButton *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventTB(CEGUI::Titlebar *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventFW(CEGUI::FrameWindow *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventGUISheet(CEGUI::GUISheet *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventComboBox(CEGUI::Combobox *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventCheckBox(CEGUI::Checkbox *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventSB(CEGUI::Scrollbar *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventLB(CEGUI::Listbox *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventEB(CEGUI::Editbox *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventThumb(CEGUI::Thumb *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventSlider(CEGUI::Slider *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventSpinner(CEGUI::Spinner *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventRadioButton(CEGUI::RadioButton *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeDragContainer(CEGUI::DragContainer *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeMultiColumnList(CEGUI::MultiColumnList *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}

// new ones the I missed in RC1
EventConnection * EventSet_subscribeWindow(CEGUI::Window *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeMouseCursor(CEGUI::MouseCursor *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeSystem(CEGUI::System *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}

#if CEGUI_VERSION_MINOR < 7 
EventConnection * EventSet_subscribeRenderer(CEGUI::Renderer *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
#endif

#if CEGUI_VERSION_MINOR > 5 && CEGUI_VERSION_PATCH > 0
EventConnection * EventSet_subscribeTree(CEGUI::Tree *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
#endif
"""

WRAPPER_DEFINITION_General = \
"""
CEGUI::ScriptModule 
General_CreateScriptModule () {
            return ( new CEGUI::ScriptModule() ) ;
            }
"""
WRAPPER_REGISTRATION_General =\
"""
bp::def( "createScriptModule", &General_CreateScriptModule,
                bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
"""

WRAPPER_DEFINITION_Window=\
"""
void
Window_setUserData ( ::CEGUI::Window & me, PyObject * data ) {
    me.setUserData ( data );
    }
    
PyObject *
Window_getUserData ( ::CEGUI::Window & me) {
    void *  data = me.getUserData (  );
    Py_INCREF( (PyObject *) data );     // I'm passing a reference to this object so better inc the ref :)
    return  (PyObject *) data;
    }
"""

WRAPPER_REGISTRATION_Window =[
    'def ("setUserData", &::Window_setUserData );',
    'def ("getUserData", &::Window_getUserData);'
    ]

WRAPPER_DEFINITION_WindowManager=\
"""
CEGUI::Window*
WindowManager_loadWindowLayout(::CEGUI::WindowManager & me,
    const CEGUI::String& filename, const CEGUI::String& name_prefix = "", const CEGUI::String& resourceGroup = "") {
      return me.loadWindowLayout( filename, name_prefix , resourceGroup);
    }

"""

WRAPPER_REGISTRATION_WindowManager =[
    'def ("loadWindowLayout", &::WindowManager_loadWindowLayout,\
        ( bp::arg("filename"), bp::arg("name_prefix")="", bp::arg("resourceGroup")="" ), \
        bp::return_value_policy< bp::reference_existing_object,bp::default_call_policies >());',
   ]
   
WRAPPER_DEFINITION_ImageSetManager =\
"""
// ovreload on create not working correctly
::CEGUI::Imageset& Imageset_create ( ::CEGUI::ImagesetManager & me, const ::CEGUI::String& xml_filename, const ::CEGUI::String& resource_group = "",
              ::CEGUI::XMLResourceExistsAction action = ::CEGUI::XREA_RETURN)
{
    return me.create(xml_filename,resource_group,action);
}
"""
WRAPPER_REGISTRATION_ImageSetManager =[
    'def ("create", &::Imageset_create,\
        ( bp::arg("xml_filename"), bp::arg("resource_group")="", bp::arg("action")=::CEGUI::XREA_RETURN ), \
        bp::return_value_policy< bp::reference_existing_object,bp::default_call_policies >());',
   ]

WRAPPER_DEFINITION_FontManager =\
"""
// ovreload on create not working correctly
::CEGUI::Font& FontManager_create ( ::CEGUI::FontManager & me, const ::CEGUI::String& xml_filename, const ::CEGUI::String& resource_group = "",
              ::CEGUI::XMLResourceExistsAction action = ::CEGUI::XREA_RETURN)
{
    return me.create(xml_filename,resource_group,action);
}
"""
WRAPPER_REGISTRATION_FontManager =[
    'def ("create", &::FontManager_create,\
        ( bp::arg("xml_filename"), bp::arg("resource_group")="", bp::arg("action")=::CEGUI::XREA_RETURN ), \
        bp::return_value_policy< bp::reference_existing_object,bp::default_call_policies >());',
   ]
   
   
def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
# #     mb.add_declaration_code( WRAPPER_DEFINITION_General )
# #     mb.add_registration_code( WRAPPER_REGISTRATION_General )
#     

    rt = mb.class_( '::CEGUI::EventSet' )
    rt.add_declaration_code( WRAPPER_DEFINITION_EventSet )
    rt.add_registration_code( WRAPPER_REGISTRATION_EventSet )

    rt = mb.class_( '::CEGUI::String' )
    rt.add_declaration_code( WRAPPER_DEFINITION_String )
    rt.add_registration_code( WRAPPER_REGISTRATION_String )

    rt = mb.class_( '::CEGUI::Window' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Window )
    apply_reg( rt,  WRAPPER_REGISTRATION_Window )

    rt = mb.class_( '::CEGUI::WindowManager' )
    rt.add_declaration_code( WRAPPER_DEFINITION_WindowManager )
    apply_reg( rt,  WRAPPER_REGISTRATION_WindowManager )

    rt = mb.class_( '::CEGUI::ImagesetManager' )
    rt.add_declaration_code( WRAPPER_DEFINITION_ImageSetManager )
    apply_reg( rt,  WRAPPER_REGISTRATION_ImageSetManager )

    rt = mb.class_( '::CEGUI::FontManager' )
    rt.add_declaration_code( WRAPPER_DEFINITION_FontManager )
    apply_reg( rt,  WRAPPER_REGISTRATION_FontManager )

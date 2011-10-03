import os
import environment

WRAPPER_REGISTRATION_Edit = \
"""

	def("subscribeEventSelectAccept", &EventEdit_subscribeSelectAccept, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	Edit_exposer.def("subscribeEventTextChange", &EventEdit_subscribeTextChange, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());

"""

WRAPPER_DEFINITION_Edit = \
"""
	#include "python_mygui_callback.h"
	
	
EventCallback* EventEdit_subscribeSelectAccept(MyGUI::Edit *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventEditSelectAccept = MyGUI::newDelegate(callback, &EventCallback::callback<MyGUI::Widget*>);
	return callback;
}

EventCallback* EventEdit_subscribeTextChange(MyGUI::Edit *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventEditTextChange = MyGUI::newDelegate(callback, &EventCallback::callback<MyGUI::Widget*>);
	return callback;
}
"""


WRAPPER_REGISTRATION_WidgetEvent = \
"""

	def("subscribeEventMouseLostFocus", &EventWidget_subscribeMouseLostFocus, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	WidgetEvent_exposer.def("subscribeEventMouseSetFocus", &EventWidget_subscribeMouseSetFocus, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	WidgetEvent_exposer.def("subscribeEventMouseDrag", &EventWidget_subscribeMouseDrag, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	WidgetEvent_exposer.def("subscribeEventMouseMove", &EventWidget_subscribeMouseMove, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	WidgetEvent_exposer.def("subscribeEventMouseWheel", &EventWidget_subscribeMouseWheel, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	WidgetEvent_exposer.def("subscribeEventMouseButtonPressed", &EventWidget_subscribeMouseButtonPressed, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	WidgetEvent_exposer.def("subscribeEventMouseButtonReleased", &EventWidget_subscribeMouseButtonReleased, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	WidgetEvent_exposer.def("subscribeEventMouseButtonClick", &EventWidget_subscribeMouseButtonClick, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	WidgetEvent_exposer.def("subscribeEventMouseButtonDoubleClick", &EventWidget_subscribeMouseButtonDoubleClick, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	WidgetEvent_exposer.def("subscribeEventKeyLostFocus", &EventWidget_subscribeKeyLostFocus, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	WidgetEvent_exposer.def("subscribeEventKeySetFocus", &EventWidget_subscribeKeyLostFocus, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	WidgetEvent_exposer.def("subscribeEventRootMouseChangeFocus", &EventWidget_subscribeRootMouseChangeFocus, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	WidgetEvent_exposer.def("subscribeEventRootKeyChangeFocus", &EventWidget_subscribeRootKeyChangeFocus, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	WidgetEvent_exposer.def("subscribeEventToolTip", &EventWidget_subscribeToolTip, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	WidgetEvent_exposer.def("subscribeEventActionInfo", &EventWidget_subscribeActionInfo, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());

	{
		typedef bp::class_< EventCallback > EventCallback_exposer_t;
		EventCallback_exposer_t EventCallback_exposer = EventCallback_exposer_t( "EventCallback" );
		bp::scope EventCallback_scope( EventCallback_exposer );
		EventCallback_exposer.def( bp::init< >() );
	}	
"""

WRAPPER_DEFINITION_WidgetEvent = \
"""

#include "python_mygui_callback.h"

EventCallback* EventWidget_subscribeMouseLostFocus(MyGUI::WidgetEvent *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventMouseLostFocus = MyGUI::newDelegate(callback, &EventCallback::callback<MyGUI::Widget*, MyGUI::Widget*>);
	return callback;
}

EventCallback* EventWidget_subscribeMouseSetFocus(MyGUI::WidgetEvent *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventMouseSetFocus = MyGUI::newDelegate(callback, &EventCallback::callback<MyGUI::Widget*, MyGUI::Widget*>);
	return callback;
}

EventCallback* EventWidget_subscribeMouseDrag(MyGUI::WidgetEvent *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventMouseDrag = MyGUI::newDelegate(callback, &EventCallback::callbackP_R_R<MyGUI::Widget*, int, int>);
	return callback;
}

EventCallback* EventWidget_subscribeMouseMove(MyGUI::WidgetEvent *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventMouseMove = MyGUI::newDelegate(callback, &EventCallback::callbackP_R_R<MyGUI::Widget*, int, int>);
	return callback;
}

EventCallback* EventWidget_subscribeMouseWheel(MyGUI::WidgetEvent *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventMouseWheel = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::Widget*, int>);
	return callback;
}

EventCallback* EventWidget_subscribeMouseButtonPressed(MyGUI::WidgetEvent *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventMouseButtonPressed = MyGUI::newDelegate(callback, &EventCallback::callbackP_R_R_R<MyGUI::Widget*, int, int, MyGUI::MouseButton>);
	return callback;
}

EventCallback* EventWidget_subscribeMouseButtonReleased(MyGUI::WidgetEvent *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventMouseButtonReleased = MyGUI::newDelegate(callback, &EventCallback::callbackP_R_R_R<MyGUI::Widget*, int, int, MyGUI::MouseButton>);
	return callback;
}

EventCallback* EventWidget_subscribeMouseButtonClick(MyGUI::WidgetEvent *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventMouseButtonClick = MyGUI::newDelegate(callback, &EventCallback::callback<MyGUI::Widget*>);
	return callback;
}

EventCallback* EventWidget_subscribeMouseButtonDoubleClick(MyGUI::WidgetEvent *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventMouseButtonDoubleClick = MyGUI::newDelegate(callback, &EventCallback::callback<MyGUI::Widget*>);
	return callback;
}

EventCallback* EventWidget_subscribeKeyLostFocus(MyGUI::WidgetEvent *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventKeyLostFocus = MyGUI::newDelegate(callback, &EventCallback::callback<MyGUI::Widget*, MyGUI::Widget*>);
	return callback;
}

EventCallback* EventWidget_subscribeKeySetFocus(MyGUI::WidgetEvent *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventKeySetFocus = MyGUI::newDelegate(callback, &EventCallback::callback<MyGUI::Widget*, MyGUI::Widget*>);
	return callback;
}

EventCallback* EventWidget_subscribeRootMouseChangeFocus(MyGUI::WidgetEvent *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventRootMouseChangeFocus = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::Widget*, bool>);
	return callback;
}

EventCallback* EventWidget_subscribeRootKeyChangeFocus(MyGUI::WidgetEvent *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventRootKeyChangeFocus = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::Widget*, bool>);
	return callback;
}

EventCallback* EventWidget_subscribeToolTip(MyGUI::WidgetEvent *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventToolTip = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::Widget*, const MyGUI::ToolTipInfo&>);
	return callback;
}

EventCallback* EventWidget_subscribeActionInfo(MyGUI::WidgetEvent *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventActionInfo = MyGUI::newDelegate(callback, &EventCallback::callbackP_R_R<MyGUI::Widget*, const std::string&, const std::string&>);
	return callback;
}

"""

WRAPPER_REGISTRATION_Canvas = \
"""

	def("subscribeEventSelectAccept", &EventCanvas_subscribePreTextureChanges, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	Canvas_exposer.def("subscribeEventTextChange", &EventCanvas_subscribeRequestUpdateCanvas, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());

"""

WRAPPER_DEFINITION_Canvas = \
"""
	#include "python_mygui_callback.h"
	
	
EventCallback* EventCanvas_subscribePreTextureChanges(MyGUI::Canvas *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventPreTextureChanges = MyGUI::newDelegate(callback, &EventCallback::callback<MyGUI::Canvas*>);
	return callback;
}

EventCallback* EventCanvas_subscribeRequestUpdateCanvas(MyGUI::Canvas *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->requestUpdateCanvas = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::Canvas*, MyGUI::Canvas::Event>);
	return callback;
}
"""

WRAPPER_REGISTRATION_ComboBox = \
"""

	def("subscribeEventAccept", &EventComboBox_subscribeComboAccept, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	ComboBox_exposer.def("subscribeEventChangePosition", &EventComboBox_subscribeComboChangePosition, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());

"""

WRAPPER_DEFINITION_ComboBox = \
"""
	#include "python_mygui_callback.h"
	
	
EventCallback* EventComboBox_subscribeComboAccept(MyGUI::ComboBox *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventComboAccept = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::ComboBox*, size_t>);
	return callback;
}

EventCallback* EventComboBox_subscribeComboChangePosition(MyGUI::ComboBox *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventComboChangePosition = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::ComboBox*, size_t>);
	return callback;
}
"""

WRAPPER_REGISTRATION_List = \
"""

	def("subscribeEventSelectAccept", &EventList_subscribeListSelectAccept, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	List_exposer.def("subscribeEventChangePosition", &EventList_subscribeListChangePosition, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	List_exposer.def("subscribeEventMouseItemActivate", &EventList_subscribeListMouseItemActivate, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	List_exposer.def("subscribeEventMouseItemFocus", &EventList_subscribeListMouseItemFocus, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	List_exposer.def("subscribeEventChangeScroll", &EventList_subscribeListChangeScroll, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());		

"""

WRAPPER_DEFINITION_List = \
"""
	#include "python_mygui_callback.h"
	
	
EventCallback* EventList_subscribeListSelectAccept(MyGUI::List *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventListSelectAccept = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::List*, size_t>);
	return callback;
}

EventCallback* EventList_subscribeListChangePosition(MyGUI::List *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventListChangePosition = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::List*, size_t>);
	return callback;
}

EventCallback* EventList_subscribeListMouseItemActivate(MyGUI::List *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventListMouseItemActivate = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::List*, size_t>);
	return callback;
}

EventCallback* EventList_subscribeListMouseItemFocus(MyGUI::List *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventListMouseItemFocus = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::List*, size_t>);
	return callback;
}

EventCallback* EventList_subscribeListChangeScroll(MyGUI::List *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventListChangeScroll = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::List*, size_t>);
	return callback;
}

"""

WRAPPER_REGISTRATION_MenuCtrl = \
"""

	def("subscribeEventAccept", &EventMenuCtrl_subscribeMenuCtrlAccept, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	MenuCtrl_exposer.def("subscribeEventClose", &EventMenuCtrl_subscribeMenuCtrlClose, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());

"""

WRAPPER_DEFINITION_MenuCtrl = \
"""
	#include "python_mygui_callback.h"
	
	
EventCallback* EventMenuCtrl_subscribeMenuCtrlAccept(MyGUI::MenuCtrl *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventMenuCtrlAccept = MyGUI::newDelegate(callback, &EventCallback::callback<MyGUI::MenuCtrl*, MyGUI::MenuItem*>);
	return callback;
}

EventCallback* EventMenuCtrl_subscribeMenuCtrlClose(MyGUI::MenuCtrl *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventMenuCtrlClose = MyGUI::newDelegate(callback, &EventCallback::callback<MyGUI::MenuCtrl*>);
	return callback;
}
"""


WRAPPER_REGISTRATION_DDContainer = \
"""

	def("subscribeEventStartDrag", &EventDDContainer_subscribeDDContainerStartDrag, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	DDContainer_exposer.def("subscribeEventRequestDrop", &EventDDContainer_subscribeDDContainerRequestDrop, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	DDContainer_exposer.def("subscribeEventDropResult", &EventDDContainer_subscribeDDContainerDropResult, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	DDContainer_exposer.def("subscribeEventChangeDDState", &EventDDContainer_subscribeDDContainerChangeDDState, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	/*DDContainer_exposer.def("subscribeEventRequestDragWidgetInfo", &EventDDContainer_subscribeDDContainerRequestDragWidgetInfo, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	DDContainer_exposer.def("subscribeEventInvalideContainer", &EventDDContainer_subscribeDDContainerInvalideContainer, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	DDContainer_exposer.def("subscribeEventUpdateDropState", &EventDDContainer_subscribeDDContainerUpdateDropState, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());		*/
	
"""

WRAPPER_DEFINITION_DDContainer = \
"""
	#include "python_mygui_callback.h"
	
	
EventCallback* EventDDContainer_subscribeDDContainerStartDrag(MyGUI::DDContainer *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventStartDrag = MyGUI::newDelegate(callback, &EventCallback::callbackP_R_R<MyGUI::DDContainer*, const MyGUI::DDItemInfo&, bool&>);
	return callback;
}

EventCallback* EventDDContainer_subscribeDDContainerRequestDrop(MyGUI::DDContainer *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventRequestDrop = MyGUI::newDelegate(callback, &EventCallback::callbackP_R_R<MyGUI::DDContainer*, const MyGUI::DDItemInfo&, bool&>);
	return callback;
}

EventCallback* EventDDContainer_subscribeDDContainerDropResult(MyGUI::DDContainer *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventDropResult = MyGUI::newDelegate(callback, &EventCallback::callbackP_R_R<MyGUI::DDContainer*, const MyGUI::DDItemInfo&, bool>);
	return callback;
}

EventCallback* EventDDContainer_subscribeDDContainerChangeDDState(MyGUI::DDContainer *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventChangeDDState = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::DDContainer*, MyGUI::DDItemState>);
	return callback;
}

/*
EventCallback* EventDDContainer_subscribeDDContainerRequestDragWidgetInfo(MyGUI::DDContainer *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->requestDragWidgetInfo = MyGUI::newDelegate(callback, &EventCallback::callbackP_P_R<MyGUI::DDContainer*, MyGUI::Widget*, MyGUI::IntCoord>);
	return callback;
}

EventCallback* EventDDContainer_subscribeDDContainerInvalideContainer(MyGUI::DDContainer *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->_eventInvalideContainer = MyGUI::newDelegate(callback, &EventCallback::callback<MyGUI::DDContainer*>);
	return callback;
}

EventCallback* EventDDContainer_subscribeDDContainerUpdateDropState(MyGUI::DDContainer *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventUpdateDropState = MyGUI::newDelegate(callback, &EventCallback::callbackP_P_R<MyGUI::DDContainer*, MyGUI::Widget, const MyGUI::DDWidgetState&>);
	return callback;
}
*/
"""

WRAPPER_REGISTRATION_Tab = \
"""

	def("subscribeEventTabChangeSelect", &EventTab_subscribeTabChangeSelect, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	
"""

WRAPPER_DEFINITION_Tab = \
"""
	#include "python_mygui_callback.h"
	
	
EventCallback* EventTab_subscribeTabChangeSelect(MyGUI::Tab *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventTabChangeSelect = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::Tab*, size_t>);
	return callback;
}

"""


WRAPPER_REGISTRATION_VScroll = \
"""

	def("subscribeEventScrollChangePosition", &EventVScroll_subscribeVScrollChangePosition, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	
"""

WRAPPER_DEFINITION_VScroll = \
"""
	#include "python_mygui_callback.h"
	
	
EventCallback* EventVScroll_subscribeVScrollChangePosition(MyGUI::VScroll *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventScrollChangePosition = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::VScroll*, size_t>);
	return callback;
}

"""


WRAPPER_REGISTRATION_RenderBox = \
"""

	def("subscribeEventUpdateViewport", &EventRenderBox_subscribeRenderBoxUpdateViewport, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	
"""

WRAPPER_DEFINITION_RenderBox = \
"""
	#include "python_mygui_callback.h"
	
	
EventCallback* EventRenderBox_subscribeRenderBoxUpdateViewport(MyGUI::RenderBox *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventUpdateViewport = MyGUI::newDelegate(callback, &EventCallback::callback<MyGUI::RenderBox*>);
	return callback;
}

"""


WRAPPER_REGISTRATION_Window = \
"""

	def("subscribeEventWindowButtonPressed", &EventWindow_subscribeWindowButtonPressed, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	Window_exposer.def("subscribeEventWindowChangeCoord", &EventWindow_subscribeWindowChangeCoord, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());

"""

WRAPPER_DEFINITION_Window = \
"""
	#include "python_mygui_callback.h"
	
	
EventCallback* EventWindow_subscribeWindowButtonPressed(MyGUI::Window *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventWindowButtonPressed = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::Window*, const std::string&>);
	return callback;
}

EventCallback* EventWindow_subscribeWindowChangeCoord(MyGUI::Window *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventWindowChangeCoord = MyGUI::newDelegate(callback, &EventCallback::callback<MyGUI::Window*>);
	return callback;
}
"""



WRAPPER_REGISTRATION_Message = \
"""

	def("subscribeEventMessageBoxResult", &EventMessage_subscribeMessageBoxResult, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	
"""

WRAPPER_DEFINITION_Message = \
"""
	#include "python_mygui_callback.h"
	
	
EventCallback* EventMessage_subscribeMessageBoxResult(MyGUI::Message *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventMessageBoxResult = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::Message*, MyGUI::MessageBoxStyle>);
	return callback;
}

"""



WRAPPER_REGISTRATION_MultiList = \
"""

	def("subscribeEventListSelectAccept", &EventMultiList_subscribeListSelectAccept, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
	MultiList_exposer.def("subscribeEventListChangePosition", &EventMultiList_subscribeListChangePosition, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());

"""

WRAPPER_DEFINITION_MultiList = \
"""
	#include "python_mygui_callback.h"
	
	
EventCallback* EventMultiList_subscribeListSelectAccept(MyGUI::MultiList *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventListSelectAccept = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::MultiList*, size_t>);
	return callback;
}

EventCallback* EventMultiList_subscribeListChangePosition(MyGUI::MultiList *self, PyObject *subscriber, Ogre::String _method)
{
	EventCallback *callback = new EventCallback(subscriber, _method);
	self->eventListChangePosition = MyGUI::newDelegate(callback, &EventCallback::callbackP_R<MyGUI::MultiList*, size_t>);
	return callback;
}

"""

#################################################################################################
#################################################################################################   
  
def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
# #     mb.add_declaration_code( WRAPPER_DEFINITION_General )
# #     mb.add_registration_code( WRAPPER_REGISTRATION_General )
#     
    rt = mb.class_( 'Edit' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Edit )
    rt.add_registration_code( WRAPPER_REGISTRATION_Edit )
	
    rt = mb.class_( 'WidgetEvent' )
    rt.add_declaration_code( WRAPPER_DEFINITION_WidgetEvent )
    rt.add_registration_code( WRAPPER_REGISTRATION_WidgetEvent )
	
    rt = mb.class_( 'Canvas' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Canvas )
    rt.add_registration_code( WRAPPER_REGISTRATION_Canvas )
	
    rt = mb.class_( 'ComboBox' )
    rt.add_declaration_code( WRAPPER_DEFINITION_ComboBox )
    rt.add_registration_code( WRAPPER_REGISTRATION_ComboBox )

    rt = mb.class_( 'List' )
    rt.add_declaration_code( WRAPPER_DEFINITION_List )
    rt.add_registration_code( WRAPPER_REGISTRATION_List )
	
    rt = mb.class_( 'MenuCtrl' )
    rt.add_declaration_code( WRAPPER_DEFINITION_MenuCtrl )
    rt.add_registration_code( WRAPPER_REGISTRATION_MenuCtrl )
	
    rt = mb.class_( 'DDContainer' )
    rt.add_declaration_code( WRAPPER_DEFINITION_DDContainer )
    rt.add_registration_code( WRAPPER_REGISTRATION_DDContainer )
	
    rt = mb.class_( 'Tab' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Tab )
    rt.add_registration_code( WRAPPER_REGISTRATION_Tab )
	
    rt = mb.class_( 'VScroll' )
    rt.add_declaration_code( WRAPPER_DEFINITION_VScroll )
    rt.add_registration_code( WRAPPER_REGISTRATION_VScroll )
	
    rt = mb.class_( 'RenderBox' )
    rt.add_declaration_code( WRAPPER_DEFINITION_RenderBox )
    rt.add_registration_code( WRAPPER_REGISTRATION_RenderBox )
	
    rt = mb.class_( 'Window' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Window )
    rt.add_registration_code( WRAPPER_REGISTRATION_Window )

    rt = mb.class_( 'Message' )
    rt.add_declaration_code( WRAPPER_DEFINITION_Message )
    rt.add_registration_code( WRAPPER_REGISTRATION_Message )
	
    print "Need to wrap events: " + "eventKeyButtonPressed, eventKeyButtonReleased, _requestGetContainer in WidgetEvent class"
	
	#Button
	
	# Canvas
	#eventPreTextureChanges			+
	#requestUpdateCanvas				+
	
	#ComboBox:
	#eventComboAccept				+
	#eventComboChangePosition			+
	
	#DDContainer
	#eventStartDrag					+
	#eventRequestDrop				+
	#eventDropResult					+
	#eventChangeDDState				+
	#requestDragWidgetInfo			
	#_eventInvalideContainer			
	#eventUpdateDropState			
	
	#Edit
	#eventEditSelectAccept			+
	#eventEditTextChange				+
	
	#HScroll
	
	#ItemBox
	#requestCreateWidgetItem
	#requestCoordItem
	#requestDrawItem
	#eventSelectItemAccept
	#eventChangeItemPosition
	#eventMouseItemActivate
	#eventNotifyItem
	
	#List
	#eventListSelectAccept				+
	#eventListChangePosition			+
	#eventListMouseItemActivate			+
	#eventListMouseItemFocus			+
	#eventListChangeScroll				+
	
	#MenuBar
	
	#MenuCtrl
	#eventMenuCtrlAccept				+
	#eventMenuCtrlClose				+
	
	#Message
	#eventMessageBoxResult			+
	
	#MultiList
	#eventListSelectAccept				+
	#eventListChangePosition			+
	#requestOperatorLess
	
	#PopupMenu
	
	#Progress
	
	#RenderBox
	#eventUpdateViewport				+
	
	#ScrollView
	
	#StaticImage
	
	#StaticText
	
	#Tab
	#eventTabChangeSelect			+
	
	#VScroll
	#eventScrollChangePosition			+
	
	#Widget
	
	#WidgetEvent
	#eventMouseLostFocus				+
	#eventMouseSetFocus 				+
	#eventMouseDrag				+
	#eventMouseMove				+
	#eventMouseWheel				+
	#eventMouseButtonPressed			+
	#eventMouseButtonReleased			+
	#eventMouseButtonClick			+
	#eventMouseButtonDoubleClick		+
	#eventKeyLostFocus				+
	#eventKeySetFocus				+
	#eventKeyButtonPressed			
	#eventKeyButtonReleased			
	#eventRootMouseChangeFocus		+
	#eventRootKeyChangeFocus			+
	#eventToolTip					+
	#eventActionInfo				+
	#_requestGetContainer
	
	#Window
	#eventWindowButtonPressed			+
	#eventWindowChangeCoord			+
	
	



    
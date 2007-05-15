#ifndef SIMPLEGUINSTATEBUTTON_H
#define SIMPLEGUINSTATEBUTTON_H

#include "SimpleGUIButton.h"
#include "SimpleGUIPrerequisites.h"

#include <vector>

namespace SimpleGUI
{
	/** Represents a button with several states.
		@remarks
		A state is defined as an image and text. For a small
		example, a checkbox would be the same as a 2 state button.
		For each state, you need to define its original, mouse
		over, and mouse down state, and any text.
		@note
		A ButtonStateEvent was created for this widget.
		@note
		NStateButton must be created by a Window widget.
	*/
	class NStateButton :
		public Button
	{
	public:
		class State
		{
		public:
			State(const Ogre::String& name, const Ogre::String& material, Ogre::String text="") :
			  mName(name),
			  mMaterial(material),
			  mText(text)
			{}
			~State() {}

			Ogre::String getMaterial() { return mMaterial; }
			Ogre::String getName() { return mName; }
			Ogre::String getText() { return mText; }
			
		protected:
			Ogre::String mName;
			Ogre::String mMaterial;
			Ogre::String mText;
		};

		NStateButton(const Ogre::String& name, const Ogre::Vector4& dimensions, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~NStateButton();

		template<typename T> void addOnStateChangedEventHandler(bool (T::*function)(const EventArgs&), T* obj)
		{
			mOnStateChangedUserEventHandlers.push_back(new MemberFunctionPointer<T>(function,obj));
		}
		void addState(const Ogre::String& name, const Ogre::String& material, Ogre::String text="");

		void clearStates();

		State* getCurrentState();
		int getIndexOfState(State* s);
		State* getNextState();
		State* getPreviousState();
		State* getState(Ogre::ushort index);
		State* getState(const Ogre::String& name);

		/**
		* Default Handler for the SGUI_MOUSE_BUTTON_UP event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onMouseButtonUp(MouseEventArgs& e);
		bool onStateChanged(WidgetEventArgs& e);

		void setCurrentState(State* s);
		void setCurrentState(Ogre::ushort index);
		void setCurrentState(const Ogre::String& name);

		void toggleNextState();
		void togglePreviousState();
		
	protected:
		std::vector<State*>	mStates;

		State* mCurrentState;

		std::vector<MemberFunctionSlot*> mOnStateChangedUserEventHandlers;
	private:
	};
}

#endif

#ifndef QUICKGUISERIALWRITER_H
#define QUICKGUISERIALWRITER_H

#include "QuickGUISerialBase.h"
#include "QuickGUIStringConverter.h"
#include "QuickGUIText.h"

#include "OgreLogManager.h"
#include "OgreSingleton.h"
#include "OgreStringConverter.h"

namespace QuickGUI
{
	class _QuickGUIExport SerialWriter :
		public SerialBase,
		public Ogre::Singleton<SerialWriter>
	{
	public:
		friend class TextDesc;
	public:
		SerialWriter();

		static SerialWriter& getSingleton(void); 
		static SerialWriter* getSingletonPtr(void);

		/**
		* Creates a ScriptDefinition.
		*/
		virtual void begin(const Ogre::String& definitionType, const Ogre::String& definitionID);

		/**
		* Adds a ScriptDefinition to the ScriptWriter.
		*/
		virtual void end();

		virtual void IO(const Ogre::String& propertyName, unsigned short* member);
		virtual void IO(const Ogre::String& propertyName, BrushFilterMode* member);
		virtual void IO(const Ogre::String& propertyName, int* member);
		virtual void IO(const Ogre::String& propertyName, unsigned int* member);
		virtual void IO(const Ogre::String& propertyName, Ogre::ColourValue* member);
		virtual void IO(const Ogre::String& propertyName, ToolBarItemLayout* member);
		virtual void IO(const Ogre::String& propertyName, Ogre::String* member);
		virtual void IO(const Ogre::String& propertyName, bool* member);
		virtual void IO(const Ogre::String& propertyName, float* member);
		virtual void IO(const Ogre::String& propertyName, Point* member);
		virtual void IO(const Ogre::String& propertyName, ProgressBarFillDirection* member);
		virtual void IO(const Ogre::String& propertyName, ProgressBarLayout* member);
		virtual void IO(const Ogre::String& propertyName, ProgressBarClippingEdge* member);
		virtual void IO(const Ogre::String& propertyName, Rect* member);
		virtual void IO(const Ogre::String& propertyName, VScrollBarButtonLayout* member);
		virtual void IO(const Ogre::String& propertyName, HScrollBarButtonLayout* member);
		virtual void IO(const Ogre::String& propertyName, Size* member);
		virtual void IO(const Ogre::String& propertyName, TextAlignment* member);
		virtual void IO(const Ogre::String& propertyName, HorizontalAnchor* member);
		virtual void IO(const Ogre::String& propertyName, VerticalAnchor* member);
		bool isSerialWriter();
		bool isSerialReader();

	protected:
	private:
	};
}

#endif

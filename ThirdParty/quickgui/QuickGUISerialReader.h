#ifndef QUICKGUISERIALREADER_H
#define QUICKGUISERIALREADER_H

#include "QuickGUISerialBase.h"
#include "QuickGUIText.h"

#include "OgreSingleton.h"
#include "OgreStringConverter.h"

namespace QuickGUI
{
	class _QuickGUIExport SerialReader :
		public SerialBase,
		public Ogre::Singleton<SerialReader>,
		public Ogre::GeneralAllocatedObject
	{
	public:
		friend class TextDesc;
		friend class Root;
	public:

		static SerialReader& getSingleton(void); 
		static SerialReader* getSingletonPtr(void);

		virtual bool begin(const Ogre::String& definitionType, const Ogre::String& definitionID);

		virtual void end();

		virtual void IO(const Ogre::String& propertyName, unsigned short* member, unsigned short defaultValue);
		virtual void IO(const Ogre::String& propertyName, BrushFilterMode* member, BrushFilterMode defaultValue);
		virtual void IO(const Ogre::String& propertyName, ConsoleLayout* member, ConsoleLayout defaultValue);
		virtual void IO(const Ogre::String& propertyName, int* member, int defaultValue);
		virtual void IO(const Ogre::String& propertyName, unsigned int* member, unsigned int defaultValue);
		virtual void IO(const Ogre::String& propertyName, ColourValue* member, ColourValue defaultValue);
		virtual void IO(const Ogre::String& propertyName, ToolBarItemLayout* member, ToolBarItemLayout defaultValue);
		virtual void IO(const Ogre::String& propertyName, Ogre::String* member, Ogre::String defaultValue);
		virtual void IO(const Ogre::String& propertyName, bool* member, bool defaultValue);
		virtual void IO(const Ogre::String& propertyName, float* member, float defaultValue);
		virtual void IO(const Ogre::String& propertyName, Point* member, Point defaultValue);
		virtual void IO(const Ogre::String& propertyName, ProgressBarFillDirection* member, ProgressBarFillDirection defaultValue);
		virtual void IO(const Ogre::String& propertyName, ProgressBarLayout* member, ProgressBarLayout defaultValue);
		virtual void IO(const Ogre::String& propertyName, ProgressBarClippingEdge* member, ProgressBarClippingEdge defaultValue);
		virtual void IO(const Ogre::String& propertyName, Rect* member, Rect defaultValue);
		virtual void IO(const Ogre::String& propertyName, VScrollBarButtonLayout* member, VScrollBarButtonLayout defaultValue);
		virtual void IO(const Ogre::String& propertyName, HScrollBarButtonLayout* member, HScrollBarButtonLayout defaultValue);
		virtual void IO(const Ogre::String& propertyName, Size* member, Size defaultValue);
		virtual void IO(const Ogre::String& propertyName, HorizontalTextAlignment* member, HorizontalTextAlignment defaultValue);
		virtual void IO(const Ogre::String& propertyName, HorizontalAnchor* member, HorizontalAnchor defaultValue);
		virtual void IO(const Ogre::String& propertyName, VerticalAnchor* member, VerticalAnchor defaultValue);
		virtual void IO(const Ogre::String& propertyName, VerticalTextAlignment* member, VerticalTextAlignment defaultValue);
		bool isSerialWriter();
		bool isSerialReader();

	protected:
		SerialReader();
		virtual ~SerialReader() {}

		Ogre::String _helperFunction(const Ogre::String& propertyName, const Ogre::String& defaultValue);

	private:
	};
}

#endif

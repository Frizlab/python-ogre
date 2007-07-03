#ifndef QUICKGUITRACKBAR_H
#define QUICKGUITRACKBAR_H

#include "QuickGUIButton.h"
#include "QuickGUILabel.h"

namespace QuickGUI
{
	/** Also known as a Slider.
		Consists of a Bar and a Button that slides
		from one end to the other.
		@remarks
		Layout (vertical/horizontal) determined by dimensions.
		@note
		Must be created by a Panel, Window, or Sheet.
	*/
	class _QuickGUIExport TrackBar :
		public Label
	{
	public:
		typedef enum Layout
		{
			QGUI_TRACKBAR_HORIZONTAL	=  0,
			QGUI_TRACKBAR_VERTICAL
		};
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The x Position, y Position, width and height of the widget.
			@param
				positionMode The GuiMetricsMode for the values given for the position. (absolute/relative/pixel)
			@param
				sizeMode The GuiMetricsMode for the values given for the size. (absolute/relative/pixel)
			@param
				material Ogre material defining the widget image.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
			@note
				Vertical or Horizontal TrackBars are derived from a comparison between width and height.
        */
		TrackBar(const Ogre::String& name, const Ogre::Vector4& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~TrackBar();

		/**
		* Add user defined event that will be called when amount of progress has changed.
		*/
		template<typename T> void addOnValueChangedEventHandler(bool (T::*function)(const EventArgs&), T* obj)
		{
			mOnValueChangedHandlers.push_back(new MemberFunctionPointer<T>(function,obj));
		}
		void addOnValueChangedEventHandler(MemberFunctionSlot* function);

		/*
		* Returns the number of divisible regions that make up the trackbar.
		*/
		int getNumRegions();
		/*
		* Returns the number of positions the slider button can be positioned at.
		*/
		int getNumTicks();
		/**
		* Returns a decimal between 0.0 and 1.0 signifying the location of the slider.
		*/
		Ogre::Real getPercentage();
		/**
		* Returns the current position of the slider button.
		*/
		int getTickPosition();

		/**
		* Sets mVisible to false.  Widgets should override this to implement how they handle
		* hiding.
		*/
		void hide();

		/**
		* Default Handler for handling progress changes.
		* Note that this event is not passed to its parents, the event is specific to this widget.
		*/
		bool onValueChanged(WidgetEventArgs& e);
		/**
		* Overridden Handler.  Allows changing of slider position by clicking on trackbar.
		*/
		bool onMouseButtonDown(MouseEventArgs& e);

		/**
		* Every region has a beginning and end, shared with neighboring regions. There will
		* always be (Region + 1) number of region boundaries (Ticks). Related to setNumTicks.
		*/
		void setNumRegions(unsigned int NumRegions);
		/**
		* Sets the number of positions Slider can be moved to. Related to setNumRegions.
		*/
		void setNumTicks(unsigned int NumTicks);
		/**
		* Sets the position of the Slider along the TrackBar.
		* NOTE: Tick Position starts at 0.
		*/
		void setTickPosition(unsigned int Tick);
		/**
		* Sets mVisible to true.  Widgets should override this to implement how they handle
		* showing.
		*/
		void show();
		/**
		* Drags the slider to the next closest region.
		*/
		bool slide(const EventArgs& e);

	protected:
		Button* mSliderButton;
		Layout mLayout;

		// used to enforce slider can only move in 1 dimension
		Ogre::Real mSliderTop;
		Ogre::Real mSliderLeft;
		Ogre::Real mRelSliderWidth;
		Ogre::Real mRelSliderHeight;

		Ogre::Real mPixelInitialSliderLeft;
		Ogre::Real mPixelInitialSliderBot;

		// number of available positions for positioning the slider.
		int mNumRegions;
		// Length between a region begin and endpoint;
		Ogre::Real mRegionLength;
		// Each region has a beggining and end. mCurrentPos tracks where slider is.
		int mCurrentPos;

		Ogre::Vector2 mCurrentSliderPos;

		std::vector<MemberFunctionSlot*> mOnValueChangedHandlers;
	};
}

#endif

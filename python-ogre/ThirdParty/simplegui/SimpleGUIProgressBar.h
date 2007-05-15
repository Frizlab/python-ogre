#ifndef SIMPLEGUIPROGRESSBAR_H
#define SIMPLEGUIPROGRESSBAR_H

#include "SimpleGUIPrerequisites.h"
#include "SimpleGUIText.h"
#include "SimpleGUIWidget.h"

namespace SimpleGUI
{
	/** Represents a vertical or horizontal bar displaying
		visual status showing a current position relative to
		the beginning and end of the bar.
		@remarks
		Useful for Life Bar's, or vertical meters.
		@note
		ProgressBars must be created by the Window class.
	*/
	class ProgressBar :
		public Widget
	{
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x Position, y Position, width and height of the widget.
			@param
				material Ogre material defining the widget image.
			@param
				show toggle visibility.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		ProgressBar(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		~ProgressBar();

		/**
		* Internal method that sets the pixel location and size of the widget.
		*/
		void _applyDimensions();
		/**
		* Internal method that converts relative dimensions to absolute and 
		* pixel dimensions, and calls _applyDimensions().  Also notifies
		* all child widgets that the dimensions have changed.
		*/
		void _notifyDimensionsChanged();
		template<typename T> void addOnProgressChangedEventHandler(bool (T::*function)(const EventArgs&), T* obj)
		{
			mOnProgressChangedHandlers.push_back(new MemberFunctionPointer<T>(function,obj));
		}
		Ogre::Real getProgress();
		/**
		* Hides the widget, including any children widgets.
		*/
		void hide();
		bool onProgressChanged(WidgetEventArgs& e);
		void setMaterial(const Ogre::String& material);
		/**
		* Sets progress.  Value should be between 0.0 and 1.0
		*/
		void setProgress(Ogre::Real progress);
		/**
		* Shows the widget, including any child widgets.
		*/
		void show();

	protected:
	private:
		Type mProgressBarType;

		Ogre::PanelOverlayElement* mBackgroundOverlayElement;
		Ogre::PanelOverlayElement* mBarOverlayElement;

		Ogre::Real mProgress;

		// Default material, displayed in its original state.
		Ogre::String mMaterial;

		std::vector<MemberFunctionSlot*> mOnProgressChangedHandlers;
	};
}

#endif

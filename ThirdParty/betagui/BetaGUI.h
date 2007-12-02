// Betajaen's GUI "BetaGUI 2.0", Version 35-uncompressed, http://www.ogre3d.org/wiki/index.php/BetaGUI
//
// Copyright (c) 2007 Robin Southern <betajaen@ihoed.com>
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#ifndef BETAGUI_H
#define BETAGUI_H

#include "Ogre.h"

#pragma pointers_to_members(full_generality, virtual_inheritance)

namespace BetaGUI {
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class GUI;
		class Style;
			struct WindowStyle;
			struct WidgetStyle;
		class Window;
			class Widget;
				class Button;
				class TextInput;
					class Slider;
						class NumericSlider;
						class StringSlider;
				class Meter;
				class Select;
		class GUIListener{};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum WindowFeatureType	{ 	WFT_NONE = 0, WFT_MOVE, WFT_RESIZE, WFT_RESIZE_AND_MOVE, WFT_ON_TOP		};
	enum FocusState			{	FS_NONE = 0, FS_SELECT, FS_SELECT_OFF, FS_DRAG							};
	enum SpecialKey			{	SK_BACKSPACE = 0, SK_ENTER, SK_TAB, SK_ESCAPE							};
	enum WindowRole			{	WR_NONE = 0, WR_DRAG, WR_RESIZE, WR_CLOSE								};
	enum WindowMode			{	WM_NONE = 0, WM_DRAG, WM_RESIZE, WM_LMB_DOWN, WM_SLIDER_MOVE			};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef void (BetaGUI::GUIListener::*ButtonMethodPtr)(BetaGUI::Button*, BetaGUI::FocusState);
	typedef void (BetaGUI::GUIListener::*SliderMethodPtr)(BetaGUI::Slider*, BetaGUI::FocusState);
	typedef void (BetaGUI::GUIListener::*TextInputMethodPtr)(BetaGUI::TextInput*, BetaGUI::FocusState);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class GUI {		friend class Window;friend class Button;friend class TextInput;
		public:
			GUI(Ogre::RenderWindow*);
			~GUI();
			
			bool injectRelMouse(int RelX, int RelY, bool selectDown);
			bool injectMouse(unsigned int ScreenX, unsigned int ScreenY, bool selectDown);
			bool injectKey(const Ogre::String& key, unsigned int x, unsigned int y);
			void injectSpecialKey(SpecialKey, unsigned int ScreenX, unsigned int ScreenY);
			
			Style*		createStyle(const Ogre::String& name);
			Window*		createWindow(int x, int y, unsigned int width, unsigned int height, const Ogre::String& Style, WindowFeatureType type, const Ogre::String& caption, GUIListener*);
			void		destroyWindow(Window *window);
			
			Ogre::OverlayContainer* createMousePointer(const Ogre::String& material, unsigned int w, unsigned int h);	
			Ogre::OverlayContainer* createOverlayFromWidgetStyle(const Ogre::String &name, const Ogre::String &style, const Ogre::String &widget, int x, int y, const Ogre::String &caption = "");
			Ogre::OverlayContainer* createOverlayFromWindowStyle(const Ogre::String &name, const Ogre::String &style, int x, int y, int w, int h);
		protected:
			void __updateCenteredOverlay(Ogre::OverlayContainer*);
			Ogre::OverlayContainer* __getOverlay(const Ogre::String&, const Ogre::String& = "Panel", int x=0, int y=0, int w=0,int h=0);

			Ogre::Overlay* mOverlay;					// Main sheet overlay
			std::vector<Window*> mWindow;				// Windows
			std::map<Ogre::String, Style*> mStyle;		// Styles
			Window *mWindowToDestroy;					// Window to destroy
			Ogre::OverlayContainer* mMousePointer;		// Mouse Pointer Overlay
			unsigned int mWindowCount, mButtonCount, mTextInputCount, mRenderWindowWidth, mRenderWindowHeight;
			Ogre::Real mWindowRatio;
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Window {		friend class Button;friend class TextInput;friend class GUI;
		public:
			Button*		createButton(int x, int y, const Ogre::String& Caption, ButtonMethodPtr);
			TextInput*  createTextInput(int x, int y, int w, const Ogre::String& intialValue, unsigned maxLength);
			Ogre::OverlayContainer* createStaticText(int x, int y, const Ogre::String& caption);
			void hide() {		mOverlay->hide();}
			void show() {		mOverlay->show();}
			bool isVisible() {	return mOverlay->isVisible();}
			void setPosition(unsigned int X,unsigned int Y);
			Ogre::Vector2 getPosition();
			Ogre::Vector2 getSize();
			void setSize(unsigned int X,unsigned int Y);
			Ogre::OverlayContainer* mOverlay;			// Overlay
			~Window();
		protected:
			Window(int x, int y, unsigned int width, unsigned int height, const Ogre::String& Style, WindowFeatureType type, const Ogre::String& caption, GUIListener*, GUI*);
			
			bool check(unsigned int x, unsigned int y, bool LMB);
			bool checkKey(const Ogre::String& key, unsigned int x, unsigned int y);
			unsigned int x,y,w,h;int dx,dy;							// Dimensions
			Button *mResizeButton,*mActiveButton,*mTitlebarButton;	// Resizer, ActiveButton, Titlebar
			TextInput* mActiveTextInput;							// Active TextInput
			GUI *mGUI;												// mGUI pointer
			std::vector<BetaGUI::Button*> mButtons;					// Buttons
			std::vector<BetaGUI::TextInput*> mTextInputs;			// TextInputs
			Ogre::String mStyle;
			WindowMode mMode;
			GUIListener* mListener;
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct WidgetStyle { 
		WidgetStyle() {WidgetStyle("");}
		WidgetStyle(const Ogre::String&);
		void fromStyle(const Ogre::String&);
		Ogre::String mBackground, mFontFace;
		unsigned int mFontSize, mDecalSize, mHeight; Ogre::ColourValue mColour; bool centerText;
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Style {
		public:
			void addStyle(Ogre::String widgetName, const Ogre::String& style);
			std::map<Ogre::String, WidgetStyle> mWidgets;
			Ogre::String mWindow_Background, mFontFace;
			unsigned int mFontSize;
			static bool _set(const Ogre::String&, std::pair<Ogre::String,Ogre::String>&, unsigned int&);
			static bool _set(const Ogre::String&, std::pair<Ogre::String,Ogre::String>&, int&);
			static bool _set(const Ogre::String&, std::pair<Ogre::String,Ogre::String>&, bool&);
			static bool _set(const Ogre::String&, std::pair<Ogre::String,Ogre::String>&, Ogre::Real&);
			static bool _set(const Ogre::String&, std::pair<Ogre::String,Ogre::String>&, Ogre::String&);
			static bool _set(const Ogre::String&, std::pair<Ogre::String,Ogre::String>&, Ogre::ColourValue&);
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Button { 
			friend class Window;
			
			void setCaption(const Ogre::String&, bool resize = false);

		protected:

			Button(int x, int y, const Ogre::String& caption, ButtonMethodPtr, Window *parent);
			Button(int x, int y, int w, int h, const Ogre::String& caption, Window *parent, WindowRole);
			~Button() {
				mOverlay->getParent()->removeChild(mOverlay->getName());
			}
			void setToActive();
			void setToNormal();

			bool out(unsigned int mx, unsigned int my, unsigned int px, unsigned int py) {
				return ( !(mx >= x + px && my >= y + py)) || ( ! (mx <= x + px + w && my <= y + py + h) );
			}

			Ogre::OverlayContainer* mOverlay, *mCaptionOverlay,*mDecalLeft,*mDecalRight;	// Main Overlay and Caption
			Ogre::String mBackground_Normal,mBackground_Active;									// MaterialName Normal, MaterialName Active
			int x,y; unsigned int w,h;														// Dimensions.
			ButtonMethodPtr mMethod;
			WindowRole mRole;
			Window *mParent;
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class TextInput {
		public:
			friend class Window;
			
		protected:
			TextInput(int x, int y, int w, const Ogre::String& intialValue, unsigned maxLength, Window *parent);
			~TextInput(){}
			Ogre::String getValue() const {
				return mValue;
			}
			
			void setValue(const Ogre::String& v) {
				mCaptionOverlay->setCaption(mValue=v);
			}

		protected:
			bool in(unsigned int mx, unsigned int my, unsigned int px, unsigned int py) {
				return ( !(mx >= x + px && my >= y + py)) || ( ! (mx <= x + px + w && my <= y + py + h) );
			}
			Ogre::OverlayContainer* mOverlay,*mCaptionOverlay,*mDecalLeft,*mDecalRight;
			Ogre::String mBackground_Normal, mBackground_Active, mValue;
			int x,y;
			unsigned int w,h, mLength;
			Window* mParent;
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Slider {
			friend class Window;
			
		public:
			
			class SliderValueRange {
				public:
					// Mark has to be between 0 and 1 (marker / width). 
//					virtual void set(Ogre::Real marker, Ogre::String &value) {}
//					virtual Ogre::Real get() {return 0;}
//					virtual Ogre::String get() {return "";}
			};

			class NumericSliderValueRange : public SliderValueRange{
				public:
					NumericSliderValueRange(Ogre::Real min, Ogre::Real max, Ogre::Real step = 1.0);
					NumericSliderValueRange(Ogre::Real minA, Ogre::Real maxA, Ogre::Real stepA, Ogre::Real minB, Ogre::Real maxB, Ogre::Real stepB);

					
					// 1. (from 0,1) 2. Value.
			//		std::map<Ogre::Real, Ogre::Real>;
			};

			class DictionarySliderValueRange {

			};

		protected:

			Slider(int x, int y, int width, SliderValueRange*, Window *parent);
			
			void setToActive();
			void setToNormal();

			bool out(unsigned int mx, unsigned int my, unsigned int px, unsigned int py) {
				return ( !(mx >= x + px && my >= y + py)) || ( ! (mx <= x + px + w && my <= y + py + h) );
			}

			Ogre::OverlayContainer* mOverlay, *mMarkerOverlay,*mDecalLeft,*mDecalRight, *mSelection;		// Main Overlay and Caption
			std::vector<Ogre::OverlayContainer*> mMarkers;
			Ogre::String mBackground_Normal,mBackground_Active;												// MaterialName Normal, MaterialName Active
			int x,y; unsigned int w, h;																		// Dimensions.
			ButtonMethodPtr		mMethod;
			bool				mHorizontalSlider;
			SliderValueRange*	mRange;
			Window *mParent;
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // End of Namespace
#endif
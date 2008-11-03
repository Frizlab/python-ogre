// BetaGUI 2.0", Version 36 http://www.ogre3d.org/wiki/index.php/BetaGUI2
//
// Copyright (c) 2008 Robin Southern <betajaen@ihoed.com>
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
typedef float NxReal;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Options.
// Use OIS, automatically sets up, mouse and key listener for you. Self injects input events (unless told not too.)
#define BETAGUI_USES_OIS 1
// Fix the bug with Visual Studio compiler, related to the class member pointer size being incorrect.
// Windows/Visual Studio only, Not required in Linux, OSX,etc.
#pragma pointers_to_members(full_generality, virtual_inheritance)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Ogre.h"
#if BETAGUI_USES_OIS
#include <ois.h>
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ButtonPtr  (BetaGUI2::ButtonMethodPtr)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace BetaGUI2 {
	
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
		class Binding;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum WindowFeatureType	{ 	WFT_NONE = 0, WFT_MOVE, WFT_RESIZE, WFT_RESIZE_AND_MOVE, WFT_ON_TOP		};
	enum FocusState			{	FS_NONE = 0, FS_SELECT, FS_SELECT_OFF, FS_DRAG							};
#if BETAGUI_USES_OIS 
	enum SpecialKey			{	SK_BACKSPACE = OIS::KC_BACK, SK_ENTER = OIS::KC_RETURN, SK_TAB = OIS::KC_TAB, SK_ESCAPE	= OIS::KC_ESCAPE};
#else
	enum SpecialKey			{	SK_BACKSPACE = 0, SK_ENTER, SK_TAB, SK_ESCAPE							};
#endif
	enum WindowRole			{	WR_NONE = 0, WR_DRAG, WR_RESIZE, WR_CLOSE								};
	enum WindowMode			{	WM_NONE = 0, WM_DRAG, WM_RESIZE, WM_LMB_DOWN, WM_SLIDER_MOVE			};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef void (BetaGUI2::GUIListener::*ButtonMethodPtr)(BetaGUI2::Button*, BetaGUI2::FocusState);
	typedef void (BetaGUI2::GUIListener::*SliderMethodPtr)(BetaGUI2::Slider*, BetaGUI2::FocusState);
	typedef void (BetaGUI2::GUIListener::*TextInputMethodPtr)(BetaGUI2::TextInput*, BetaGUI2::FocusState);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class GUI :
#if BETAGUI_USES_OIS
		public OIS::KeyListener, public OIS::MouseListener, public Ogre::FrameListener
#endif
	{		friend class Window;friend class Button;friend class TextInput;
		public:
			~GUI();
#if BETAGUI_USES_OIS
			GUI(Ogre::RenderWindow*, OIS::InputManager*);
			bool mouseMoved(const OIS::MouseEvent &);
			bool mousePressed(const OIS::MouseEvent &,OIS::MouseButtonID);
			bool mouseReleased(const OIS::MouseEvent &,OIS::MouseButtonID);
			bool keyPressed(const OIS::KeyEvent &);
			bool keyReleased(const OIS::KeyEvent &);
			bool frameEnded(const Ogre::FrameEvent &evt);
			void setCapturing(bool c) {mCapturing = c;}
			bool isCaptuing() const {return mCapturing;}
			bool isOver() const{return mOver;}

#else
			GUI(Ogre::RenderWindow*);
#endif

			bool injectRelMouse(int RelX, int RelY, bool selectDown);
			bool injectMouse(unsigned int ScreenX, unsigned int ScreenY, bool selectDown);
			bool injectKey(const Ogre::String& key);
			void injectSpecialKey(SpecialKey);
	
			Style*		createStyle(const Ogre::String& name);
			Window*		createWindow(int x, int y, unsigned int width, unsigned int height, const Ogre::String& Style, WindowFeatureType type, const Ogre::String& caption, GUIListener*);
			void		destroyWindow(Window *window);
			void		setActiveWindow(Window*);
			void		hideAll();
			Ogre::OverlayContainer* createMousePointer(const Ogre::String& material, unsigned int w, unsigned int h);	
			Ogre::OverlayContainer* createOverlayFromWidgetStyle(const Ogre::String &name, const Ogre::String &style, const Ogre::String &widget, int x, int y, const Ogre::String &caption = "");
			Ogre::OverlayContainer* createOverlayFromWindowStyle(const Ogre::String &name, const Ogre::String &style, int x, int y, int w, int h);
			Ogre::Overlay* mOverlay;					// Main sheet overlay	
			Ogre::OverlayContainer* mMousePointer;		// Mouse Pointer Overlay
			Style*		getStyle(const Ogre::String& name) {return mStyle[name];}
	protected:
			void __updateCenteredOverlay(Ogre::OverlayContainer*);
			Ogre::OverlayContainer* __getOverlay(const Ogre::String&, const Ogre::String& = "Panel", int x=0, int y=0, int w=0,int h=0);
			std::vector<Window*> mWindow;				// Windows
			std::map<Ogre::String, Style*> mStyle;		// Styles
			Window *mWindowToDestroy;					// Window to destroy
			Window *mActiveWindow;						// Active Window to send special commands to and to work with.
			unsigned int mWindowCount, mButtonCount, mTextInputCount, mRenderWindowWidth, mRenderWindowHeight;
			Ogre::Real mWindowRatio;
			int lastKnownMouseX, lastKnownMouseY;
#if BETAGUI_USES_OIS
			OIS::InputManager* mInputManager;
			OIS::Mouse* mMouse;
			OIS::Keyboard* mKeyboard;
			bool mCapturing, mOver;
#endif
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
			Ogre::String		getStyle()	{return mStyle;}
			GUI*				getGUI()	{return mGUI;}
			void setToNormal(); void setToActive();
			Ogre::Vector2 getPosition();
			Ogre::Vector2 getSize();
			void setSize(unsigned int X,unsigned int Y);
			Ogre::OverlayContainer* mOverlay;			// Overlay
			void toFront();		void toZ();
			void setZ(int Z);	int getZ(){return z;}
			Window& cursor(){return *this;}
			Window& autoSize(bool as) {mCursorAutoSize = as;return *this;}
			Window& lineBreak() {mCursorX=1;mCursorY+=mCursorLineHeight;return *this;}
			Window& lineBreak(unsigned int height) {mCursorX=1;mCursorY+=height;return *this;}
			Window& left(){mCursorLeftRight = true;mCursorX=1;return *this;}
			Window& right(){mCursorLeftRight = false;mCursorX=1;return *this;}
			Window& center(){mCursorCenter = true;mCursorX=0;return *this;}
			Window& bottom() {mCursorY = -1;return *this;}
			Window& tab(unsigned int length = 25){if (mCursorX==0){mCursorX=25;return *this;}
				float t = length / 25.0f;mCursorX = int(t<0?ceil((t)-0.5):floor((t)+0.5));return *this;}
			Window& button(std::string caption, BetaGUI2::ButtonMethodPtr bmptr);
			Window& input(std::string value, unsigned int length, NxReal widgetLength = 0);
			Window& text(std::string caption);
			~Window();
		protected:
			Window(int x, int y, unsigned int width, unsigned int height, const Ogre::String& Style, WindowFeatureType type, const Ogre::String& caption, GUIListener*, GUI*);
			bool check(unsigned int x, unsigned int y, bool LMB);
			bool checkKey(const Ogre::String& key, unsigned int x, unsigned int y);
			bool checkSpecialKey(SpecialKey key,unsigned int x,unsigned int y);
			void _AdjustCursor(unsigned int nx, unsigned int nh);
			unsigned int x,y,w,h,z;int dx,dy;							// Dimensions
			Button	*mResizeButton,*mActiveButton,
					*mTitlebarButton, *mOkayButton, *mCancelButton;	// Resizer, ActiveButton, Titlebar
			TextInput* mActiveTextInput;							// Active TextInput
			GUI *mGUI;												// mGUI pointer
			std::vector<BetaGUI2::Button*> mButtons;					// Buttons
			std::vector<BetaGUI2::TextInput*> mTextInputs;			// TextInputs
			Ogre::String mStyle;
			WindowMode mMode;
			GUIListener* mListener;
			Ogre::Overlay* mSheet;									// Main sheet overlay	
			Ogre::OverlayContainer* bl,*br,*bb,*mWidgetContainer;
			int mCursorX, mCursorY, mCursorLineHeight;
			bool mCursorAutoSize, mCursorLeftRight, mCursorCenter;
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
			Ogre::String mWindow_Background, mWindow_ActiveBackground, mFontFace;
			bool mWindow_Border;
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
		public:
			void setCaption(const Ogre::String& s) {if (mCaptionOverlay)mCaptionOverlay->setCaption(s);}
			Ogre::String getCaption() const {return mCaptionOverlay->getCaption();}
			Ogre::OverlayContainer*	getMainOverlay(){return mOverlay;}
			unsigned int getWidth();	unsigned int getHeight();
			int getX(){return x;}int getY(){return y;}void setX(int x);void setY(int y);
		protected:
			Button(int x, int y, const Ogre::String& caption, ButtonMethodPtr, Window *parent);
			Button(int x, int y, int w, int h, const Ogre::String& caption, Window *parent, WindowRole);
			~Button(){mOverlay->getParent()->removeChild(mOverlay->getName());}
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
			
			void setToActive();
			void setToNormal();

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
// 	class Slider {
// 			friend class Window;
// 			
// 		public:
// 			
// 			class SliderValueRange {
// 				public:
// 					// Mark has to be between 0 and 1 (marker / width). 
// //					virtual void set(Ogre::Real marker, Ogre::String &value) {}
// //					virtual Ogre::Real get() {return 0;}
// //					virtual Ogre::String get() {return "";}
// 			};
// 
// // 			class NumericSliderValueRange : public SliderValueRange{
// // 				public:
// // 					NumericSliderValueRange(Ogre::Real min, Ogre::Real max, Ogre::Real step = 1.0);
// // 					NumericSliderValueRange(Ogre::Real minA, Ogre::Real maxA, Ogre::Real stepA, Ogre::Real minB, Ogre::Real maxB, Ogre::Real stepB);
// // 
// // 					
// // 					// 1. (from 0,1) 2. Value.
// // 			//		std::map<Ogre::Real, Ogre::Real>;
// // 			};
// 
// 			class DictionarySliderValueRange {
// 
// 			};
// 
// 		protected:
// 
// 			Slider(int x, int y, int width, SliderValueRange*, Window *parent);
// 			
// 			void setToActive();
// 			void setToNormal();
// 
// 			bool out(unsigned int mx, unsigned int my, unsigned int px, unsigned int py) {
// 				return ( !(mx >= x + px && my >= y + py)) || ( ! (mx <= x + px + w && my <= y + py + h) );
// 			}
// 
// 			Ogre::OverlayContainer* mOverlay, *mMarkerOverlay,*mDecalLeft,*mDecalRight, *mSelection;		// Main Overlay and Caption
// 			std::vector<Ogre::OverlayContainer*> mMarkers;
// 			Ogre::String mBackground_Normal,mBackground_Active;												// MaterialName Normal, MaterialName Active
// 			int x,y; unsigned int w, h;																		// Dimensions.
// 			ButtonMethodPtr		mMethod;
// 			bool				mHorizontalSlider;
// 			SliderValueRange*	mRange;
// 			Window *mParent;
// 	};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // End of Namespace
#endif



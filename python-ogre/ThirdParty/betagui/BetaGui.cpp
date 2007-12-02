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

#include "BetaGUI.h"
#include "OgreFontManager.h"
#include "OgreFont.h"
#include "OgreTextAreaOverlayElement.h"

namespace BetaGUI {
	////////////////////////////////////////////////////////////////////////////////////
	WidgetStyle::WidgetStyle(const Ogre::String &style) {
		mBackground = "default"; centerText = false; mColour = Ogre::ColourValue::White; mDecalSize = 0;
		mHeight = 16; mFontSize = 0; mFontFace = ""; if (style.length()>=0) fromStyle(style);}
	////////////////////////////////////////////////////////////////////////////////////
	GUI::GUI(Ogre::RenderWindow* w) : mWindowToDestroy(0), mMousePointer(0), mWindowCount(0), mButtonCount(0),
		mTextInputCount(0), mRenderWindowWidth(w->getWidth()), mRenderWindowHeight(w->getHeight()),
		mWindowRatio(Ogre::Real(mRenderWindowWidth) / Ogre::Real(mRenderWindowHeight)) {
			mOverlay = Ogre::OverlayManager::getSingleton().create("BetaGUI");
			mOverlay->show();}
	////////////////////////////////////////////////////////////////////////////////////
	GUI::~GUI() {
		for(unsigned int i=0;i < mWindow.size();i++) delete mWindow[i];
		for (std::map<Ogre::String, Style*>::iterator it = mStyle.begin();it!=mStyle.end();it++) delete (*it).second;
		mStyle.clear(); mWindow.clear();}
	////////////////////////////////////////////////////////////////////////////////////
	bool GUI::injectRelMouse(int x, int y, bool LMB) {
		return injectMouse(mMousePointer->getLeft() + x, mMousePointer->getTop() + y, LMB);}
	////////////////////////////////////////////////////////////////////////////////////
	bool GUI::injectMouse(unsigned int x, unsigned int y, bool LMB) {
		
		if (mMousePointer)
			mMousePointer->setPosition(x,y);

		for(std::vector<Window*>::iterator i=mWindow.begin();i!=mWindow.end();i++) {
			if (mWindowToDestroy == (*i)) {
				delete (*i);
				mWindow.erase(i);
				mWindowToDestroy = 0;
				return false;
			}

			if((*i)->check(x,y,LMB)) {
				return true;
			}
		}
		
		return false;
	}
	////////////////////////////////////////////////////////////////////////////////////
	bool GUI::injectKey(const Ogre::String& key, unsigned int x, unsigned int y) {
		for(unsigned int i=0;i<mWindow.size();i++) {
			if(mWindow[i]->checkKey(key,x,y)) {
				return true;}}
		return false;
	}
	////////////////////////////////////////////////////////////////////////////////////
/*	void GUI::injectBackspace(unsigned int x, unsigned int y) {
		injectKey("!b", x, y);
	}
*/
	////////////////////////////////////////////////////////////////////////////////////
	Ogre::OverlayContainer* GUI::__getOverlay(const Ogre::String& name, const Ogre::String& type,int x,int y, int w, int h) {
		Ogre::OverlayContainer* oc; try { oc = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().getOverlayElement(name));
		}catch(...) {oc = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement(type, name));}
		oc->setMetricsMode(Ogre::GMM_PIXELS);
		if (x==0&&y==0&&w==0)return oc; oc->setPosition(x,y); oc->setDimensions(w,h); return oc;
	}
	////////////////////////////////////////////////////////////////////////////////////
	Ogre::OverlayContainer* GUI::createOverlayFromWidgetStyle(const Ogre::String &name, const Ogre::String &styleName, const Ogre::String &widgetType, int x, int y, const Ogre::String &text) {
		WidgetStyle ws = mStyle[styleName]->mWidgets[widgetType];
		Ogre::OverlayContainer* oc = __getOverlay(name,"Panel", x,y, 32, ws.mHeight);
		oc->setMaterialName(ws.mBackground);
		if (text.length() >= 1) {
			int fontSize = ws.mFontSize ? ws.mFontSize : mStyle[styleName]->mFontSize;
			Ogre::String fontName = ws.mFontFace.length() ? ws.mFontFace : mStyle[styleName]->mFontFace;
			Ogre::OverlayContainer* caption = __getOverlay(name + "-caption", "TextArea", 0,((ws.mHeight * 0.5f) - ((float) fontSize) * 0.5f) * mWindowRatio, 32, fontSize);
			oc->addChild(caption);
			caption->setCaption(text);caption->setParameter("font_name", fontName);caption->setParameter("char_height", Ogre::StringConverter::toString(fontSize));
			Ogre::Font* f = dynamic_cast<Ogre::Font*>(Ogre::FontManager::getSingletonPtr()->getByName(fontName).get());
			f->setTrueTypeSize(fontSize);	float textWidth = 0; Ogre::Real sp = f->getGlyphAspectRatio(0x0030) * fontSize;
			for(unsigned int i=0; i < text.length();i++){	
				if (text[i] == 0x0020)	textWidth += sp;
				else textWidth += f->getGlyphAspectRatio(text[i]) * fontSize;}
			oc->setWidth(Ogre::Math::Ceil(textWidth * 1.25f));	caption->setWidth(textWidth); caption->setHeight(fontSize);
			if (ws.centerText) caption->setLeft(textWidth * 0.125f);
			caption->setColour(ws.mColour);}
		return oc;
	}
	////////////////////////////////////////////////////////////////////////////////////
	Ogre::OverlayContainer* GUI::createOverlayFromWindowStyle(const Ogre::String &name, const Ogre::String &styleName, int x, int y, int w, int h) {
		Ogre::OverlayContainer* oc = __getOverlay(name);
		oc->setPosition(x,y);oc->setDimensions(w,h);oc->setMaterialName(mStyle[styleName]->mWindow_Background);
		return oc;
	}
	////////////////////////////////////////////////////////////////////////////////////
	void GUI::__updateCenteredOverlay(Ogre::OverlayContainer* overlay) {
		Ogre::OverlayContainer* caption = static_cast<Ogre::OverlayContainer*>(overlay->getChild(overlay->getName() + "-caption"));
		caption->setLeft((overlay->getWidth() * 0.5f) - (caption->getWidth() * 0.5f));
		caption->setTop(2 + (overlay->getHeight() * 0.5f) - (caption->getHeight() * 0.5f));
	}
	////////////////////////////////////////////////////////////////////////////////////
	Ogre::OverlayContainer* GUI::createMousePointer(const Ogre::String& m, unsigned int w, unsigned int h) {
		Ogre::Overlay* o = Ogre::OverlayManager::getSingleton().create("BetaGUI.MP");
		mMousePointer = __getOverlay("BetaGUI.MousePointer", "Panel", mRenderWindowWidth * 0.5f, mRenderWindowHeight * 0.5f, w, h);
		mMousePointer->setMaterialName(m);
		o->setZOrder(649); o->add2D(mMousePointer); o->show(); mMousePointer->show();
		return mMousePointer;
	}
	////////////////////////////////////////////////////////////////////////////////////
	Style*	GUI::createStyle(const Ogre::String& name) {
		mStyle[name] = new Style;
		return mStyle[name];
	}
	////////////////////////////////////////////////////////////////////////////////////
	Window*	 GUI::createWindow(int x, int y, unsigned int width, unsigned int height, const Ogre::String& style, WindowFeatureType type, const Ogre::String& caption, GUIListener* listener) {
		Window* window = new BetaGUI::Window(x,y,width,height, style,type,caption, listener, this);
		mWindow.push_back(window);
		return window;
	}
	////////////////////////////////////////////////////////////////////////////////////
	void GUI::destroyWindow(Window *window) {
		mWindowToDestroy=window;}
	////////////////////////////////////////////////////////////////////////////////////
	Window::Window(int X, int Y, unsigned int width, unsigned int height, const Ogre::String& style, WindowFeatureType type, const Ogre::String& caption, GUIListener* listener, GUI* gui) 
		: x(X), y(Y), w(width), h(height), mGUI(gui), mTitlebarButton(0), mResizeButton(0), mActiveTextInput(0), mActiveButton(0), mStyle(style), mMode(WM_NONE), mListener(listener)
	{

		const Ogre::String overlayID = gui->mOverlay->getName() + "/window;" + Ogre::StringConverter::toString(gui->mWindowCount++);
		mOverlay = gui->createOverlayFromWindowStyle(overlayID,style,x,y,w,h);
		gui->mOverlay->add2D(mOverlay);
		mOverlay->show();

		if (type == WFT_RESIZE || type == WFT_RESIZE_AND_MOVE) {
	//		Callback c;c.t=4;
	//		mResizeButton=createButton(Ogre::Vector4(width-16,height-16,16,16),"bgui.button", "",c);
	//		mResizeButton = new Button(0,0,mWidth,0, caption, this, WR_RESIZE);
//			mButtons.push_back(mResizeButton);
		}

		if (type == WFT_MOVE || type == WFT_RESIZE_AND_MOVE) {
			mTitlebarButton = new Button(0,0,w,32,caption, this, WR_DRAG);
			mButtons.push_back(mTitlebarButton);
		}

	}
	////////////////////////////////////////////////////////////////////////////////////
	Window::~Window() {
		for(unsigned int i=0;i<mButtons.size();i++)			delete mButtons[i];
		for(unsigned int i=0;i<mTextInputs.size();i++)		delete mTextInputs[i];
		mGUI->mOverlay->remove2D(mOverlay);
	}
	////////////////////////////////////////////////////////////////////////////////////
	void Window::setPosition(unsigned int X,unsigned int Y) {
		mOverlay->setPosition(x=X,y=Y);
	}
	////////////////////////////////////////////////////////////////////////////////////
	Ogre::Vector2 Window::getPosition() {	return Ogre::Vector2(x,y);		}
	Ogre::Vector2 Window::getSize()		{	return Ogre::Vector2(w,h);		}
	////////////////////////////////////////////////////////////////////////////////////
	void Window::setSize(unsigned int X,unsigned int Y) {
		mOverlay->setDimensions(w=X,h=Y);
		if (mResizeButton)			mResizeButton->mOverlay->setPosition(mResizeButton->x=w-16,mResizeButton->y=h-16);
		if(mTitlebarButton)			mTitlebarButton->mOverlay->setWidth(mTitlebarButton->w=w);}
	////////////////////////////////////////////////////////////////////////////////////
	Button* Window::createButton(int x, int y, const Ogre::String& caption, ButtonMethodPtr method) {
		Button *button = new Button(x,y, caption, method, this);
		mButtons.push_back(button);
		return button;
	}
	////////////////////////////////////////////////////////////////////////////////////
	Button::Button(int X, int Y, const Ogre::String& caption, ButtonMethodPtr method, Window *parent)
		:x(X), y(Y), mMethod(method), mParent(parent), mOverlay(0), mCaptionOverlay(0), mDecalLeft(0), mDecalRight(0), mRole(WR_NONE)
	{

		int decalWidth = parent->mGUI->mStyle[parent->mStyle]->mWidgets["button"].mDecalSize;
		Ogre::String overlayID = parent->mOverlay->getName() + "/button;" + Ogre::StringConverter::toString(parent->mGUI->mButtonCount++);
		mOverlay = mParent->mGUI->createOverlayFromWidgetStyle(overlayID, parent->mStyle, "button", x + decalWidth, y, caption);

		h = mOverlay->getHeight();

		parent->mOverlay->addChild(mOverlay);
		mOverlay->show();

		mBackground_Normal = mOverlay->getMaterialName();
		mBackground_Active = mBackground_Normal + "-active";
		
		if (decalWidth != 0){	mOverlay->addChild(mDecalLeft = parent->mGUI->__getOverlay(mOverlay->getName() + ",L", "Panel", -decalWidth,0, decalWidth, mOverlay->getHeight()));
								mDecalLeft->setMaterialName(mOverlay->getMaterialName() + "L");
								mOverlay->addChild(mDecalRight = parent->mGUI->__getOverlay(mOverlay->getName() + ",R", "Panel", mOverlay->getWidth(),0, decalWidth, mOverlay->getHeight())); 
								mDecalRight->setMaterialName(mOverlay->getMaterialName() + "R");
								w = mOverlay->getWidth() + mDecalLeft->getWidth();}

		else{					w = mOverlay->getWidth();}

		if (caption.length() == 0) return;
		if (x < 0){		x = parent->getSize().x - w - Ogre::Math::Abs(x);
						mOverlay->setLeft(x);}

		if (y < 0){		y = parent->getSize().y - h - Ogre::Math::Abs(y);
						mOverlay->setTop(y);}

	}
	////////////////////////////////////////////////////////////////////////////////////
	Button::Button(int X, int Y, int W, int H, const Ogre::String& caption, Window *parent, WindowRole role)
		:x(X), y(Y), w(W), h(H), mParent(parent), mOverlay(0), mCaptionOverlay(0), mDecalLeft(0),
		mDecalRight(0), mRole(role) {

		Ogre::String overlayID = parent->mOverlay->getName() + "/titlebar";
		parent->mGUI->mButtonCount++;
		if (mRole == WR_DRAG) {
			mOverlay = mParent->mGUI->createOverlayFromWidgetStyle(overlayID, parent->mStyle, "titlebar", x, y, caption);
			parent->mOverlay->addChild(mOverlay);
			mOverlay->setWidth(W);
			parent->mGUI->__updateCenteredOverlay(mOverlay);
			mOverlay->show();
			
			w = mOverlay->getWidth();
			h = mOverlay->getHeight();
			mBackground_Normal = mOverlay->getMaterialName();
			mBackground_Active = mBackground_Normal;
			return;
		}

	}
	////////////////////////////////////////////////////////////////////////////////////
	void Button::setToActive() {
		mOverlay->setMaterialName(mBackground_Active);
		if (mDecalLeft == 0)
			return;
		mDecalLeft->setMaterialName(mBackground_Active + "L");
		mDecalRight->setMaterialName(mBackground_Active + "R");
	}
	////////////////////////////////////////////////////////////////////////////////////
	void Button::setToNormal() {
		mOverlay->setMaterialName(mBackground_Normal);
		if (mDecalLeft == 0)
			return;
		mDecalLeft->setMaterialName(mBackground_Normal + "L");
		mDecalRight->setMaterialName(mBackground_Normal + "R");
	}
	////////////////////////////////////////////////////////////////////////////////////
	TextInput* Window::createTextInput(int x, int y, int w, const Ogre::String& initialValue, unsigned maxLength) {
		TextInput *textinput = new TextInput(x,y,w, initialValue, maxLength, this);
		mTextInputs.push_back(textinput);
		return textinput;
	}
	////////////////////////////////////////////////////////////////////////////////////
	Ogre::OverlayContainer* Window::createStaticText(int x, int y, const Ogre::String& caption) {
		Ogre::OverlayContainer* text = mGUI->__getOverlay(mOverlay->getName() + "/text;" + Ogre::StringConverter::toString(mGUI->mButtonCount++), "TextArea", x,y,64,30);
		text->setCaption(caption);
		text->setParameter("font_name", mGUI->mStyle[mStyle]->mFontFace);
		text->setParameter("char_height", Ogre::StringConverter::toString(mGUI->mStyle[mStyle]->mFontSize));
		text->show();
		mOverlay->addChild(text);
		return text;
	}
	////////////////////////////////////////////////////////////////////////////////////
	TextInput::TextInput(int X, int Y, int W, const Ogre::String& initialValue, unsigned maxLength, Window *parent)
		: x(X), y(Y), mValue(initialValue), mLength(maxLength), mParent(parent)
	//	: x(D.x),y(D.y),w(D.z),h(D.w),value(V),mBackground_Normal(M), mBackground_Active(M), length(L) 
	{
		int decalWidth = parent->mGUI->mStyle[parent->mStyle]->mWidgets["textinput"].mDecalSize;
		
		Ogre::String overlayID = parent->mOverlay->getName() + "/textinput;" + Ogre::StringConverter::toString(parent->mGUI->mTextInputCount++);
		mOverlay = mParent->mGUI->createOverlayFromWidgetStyle(overlayID, parent->mStyle, "textinput", x + 0, y, initialValue);
		parent->mOverlay->addChild(mOverlay);
		mOverlay->show();

		if (mOverlay->getWidth() < W) {
			mOverlay->setWidth(W);
		}


		if (decalWidth != 0) {
			mDecalLeft = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", mOverlay->getName() + ",L"));
			mOverlay->addChild(mDecalLeft);
			mDecalLeft->setMetricsMode(Ogre::GMM_PIXELS);
			mDecalLeft->setPosition(-decalWidth,0);
			mDecalLeft->setDimensions(decalWidth,mOverlay->getHeight());
			mDecalLeft->setMaterialName(mOverlay->getMaterialName() + "L");

			mDecalRight = static_cast<Ogre::OverlayContainer*>(mDecalLeft->clone(mOverlay->getName() + ",R"));
			mOverlay->addChild(mDecalRight);
			mDecalRight->setLeft(mOverlay->getWidth());
			mDecalRight->setMaterialName(mOverlay->getMaterialName() + "R");
		
			w = mOverlay->getWidth() + mDecalLeft->getWidth();
		}
		else {
			w = mOverlay->getWidth();
		}
		h = mOverlay->getHeight();


		mBackground_Normal = mOverlay->getMaterialName();
		mBackground_Active = mBackground_Normal + "-active";
	}
	////////////////////////////////////////////////////////////////////////////////////
	bool Window::checkKey(const Ogre::String& k, unsigned int px, unsigned int py) {
		
		if (!mOverlay->isVisible())
			return false;

		if(!(px>=x&&py>=y)||!(px<=x+w&&py<=y+h))return false;
		
		if(mActiveTextInput == 0)
			return false;
		
		if(k=="!b") {
			mActiveTextInput->setValue(mActiveTextInput->mValue.substr(0,mActiveTextInput->mValue.length()-1));
			return true;
		}

		if(mActiveTextInput->mValue.length() >= mActiveTextInput->mLength)
			return true;
		
		mActiveTextInput->mCaptionOverlay->setCaption(mActiveTextInput->mValue+=k);
		return true;
	}
	////////////////////////////////////////////////////////////////////////////////////
	bool Window::check(unsigned int px, unsigned int py, bool LMB) {
		if (!mOverlay->isVisible())	return false;
		if (mMode == WM_DRAG) {
			if (LMB){	setPosition(px + dx, py + dy);
			}else		mMode = WM_NONE;
			return true;}
		if (mMode == WM_LMB_DOWN){
				if (!LMB){	(mListener->*mActiveButton->mMethod)(mActiveButton, FS_SELECT_OFF);
								mMode = WM_NONE;}
				else{	return false;}}
		if (!(px >= x && py >= y) || !(px <= x + w && py <= y + h)){
			if (mActiveButton){		mActiveButton->setToNormal();
									mActiveButton = 0;}
			return false;}
		
		for (unsigned int i=0;i < mButtons.size();i++) {

			if (mButtons[i]->out(px,py,x,y)) {
				if (mActiveButton) {
					mActiveButton->setToNormal();
					mActiveButton = 0;
				}
				continue;
			}
		
			mActiveButton=mButtons[i];

			if (LMB) {
				mActiveButton->setToActive();

				if (mActiveButton->mRole == WR_NONE) {
					mMode = WM_LMB_DOWN;
					(mListener->*mActiveButton->mMethod)(mActiveButton, FS_SELECT);
				}
				else if(mActiveButton->mRole==WR_DRAG) {
					mMode = WM_DRAG;
					dx = (int) x - px;	dy = (int) y - py;
					return true;
				}
				// Call function
				// Some how make sure it only calls it once.
			}
			else {
				mActiveButton->setToNormal();
			}

			if(mActiveTextInput && LMB) {
				// mActiveTextInput->setToNormal();
				mActiveTextInput=0;
			}

			return true;
		}

		if (!LMB)
			return false;

		for (unsigned int i=0;i<mTextInputs.size();i++) {
	
			if (mTextInputs[i]->in(px,py,x,y))
				continue;

			mActiveTextInput=mTextInputs[i];
			mActiveTextInput->mOverlay->setMaterialName(mActiveTextInput->mBackground_Active);
			return true;
		}
		
		if(mActiveTextInput) {
			mActiveTextInput->mOverlay->setMaterialName(mActiveTextInput->mBackground_Normal);
			mActiveTextInput=0;
			return true;
		}
		
		return false;
	}
	////////////////////////////////////////////////////////////////////////////////////
	void Style::addStyle(Ogre::String widgetName, const Ogre::String& style) {
		std::transform(widgetName.begin(),widgetName.end(),widgetName.begin(),tolower);
		mWidgets[widgetName] = WidgetStyle(style);
}
	////////////////////////////////////////////////////////////////////////////////////
	void WidgetStyle::fromStyle(const Ogre::String& p) {
		static const std::string delims = " \t\r";
		std::vector<std::string> ds = Ogre::StringUtil::split(p,";");
		std::pair<std::string,std::string> d;
		size_t dp;
		for(std::vector<std::string>::iterator it = ds.begin();it != ds.end();++it) {
			dp = (*it).find_first_of(":");
			d.first = (*it).substr(0,dp);d.second = (*it).substr(dp+1, (*it).length() - dp - 1);
			d.first.erase(d.first.find_last_not_of(delims)+1);d.first.erase(0,d.first.find_first_not_of(delims));
			d.second.erase(d.second.find_last_not_of(delims)+1);d.second.erase(0,d.second.find_first_not_of(delims));
			std::transform(d.first.begin(),d.first.end(),d.first.begin(),tolower);
			if (Style::_set("background", d, mBackground)) continue;
			if (Style::_set("font-face", d, mFontFace)) continue;
			if (Style::_set("font-size", d, mFontSize)) continue;
			if (Style::_set("decal-size", d, mDecalSize)) continue;
			if (Style::_set("height", d, mHeight)) continue;
			if (Style::_set("colour", d, mColour)) continue;
			if (Style::_set("color", d, mColour)) continue;
			if (d.first == "text-align"){
				std::transform(d.second.begin(),d.second.end(),d.second.begin(),tolower);
				if(d.second=="centre"||d.second=="center"){centerText=true;}
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////
	bool Style::_set(const Ogre::String& i, std::pair<Ogre::String,Ogre::String>& d , unsigned int& t) {
		if (d.first == i) {	t = Ogre::StringConverter::parseUnsignedInt(d.second); return true;}return false;}
	bool Style::_set(const Ogre::String& i, std::pair<Ogre::String,Ogre::String>& d , int& t) {
		if (d.first == i) {	t = Ogre::StringConverter::parseInt(d.second); return true;} return false;}
	bool Style::_set(const Ogre::String& i, std::pair<Ogre::String,Ogre::String>& d, bool& t) {
		if (d.first == i) {	t = Ogre::StringConverter::parseBool(d.second); return true;} return false;}
	bool Style::_set(const Ogre::String& i , std::pair<Ogre::String,Ogre::String>& d, Ogre::Real& t) {
		if (d.first == i) {	t = Ogre::StringConverter::parseReal(d.second);	return true;}return false;}
	bool Style::_set(const Ogre::String& i, std::pair<Ogre::String,Ogre::String>& d, Ogre::String& t) {
		if (d.first == i) {	t = d.second;return true;}return false;}
	bool Style::_set(const Ogre::String& i, std::pair<Ogre::String,Ogre::String>& d, Ogre::ColourValue& t) {
		if (d.first == i) {	t = Ogre::StringConverter::parseColourValue(d.second);return true;}return false;}
	////////////////////////////////////////////////////////////////////////////////////
	
} // End of Betajaen's GUI. Normal programming can resume now. 
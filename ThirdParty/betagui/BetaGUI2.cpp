// Betajaen's GUI "BetaGUI2 2.0", Version 35-uncompressed, http://www.ogre3d.org/wiki/index.php/BetaGUI2
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
// OF MERCHANTABILITY, FITNESS FOR A PAreturn trueICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TOreturn true OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#include "BetaGUI2.h"
#include "OgreFontManager.h"
#include "OgreFont.h"
#include "OgreTextAreaOverlayElement.h"

#define return_this_if(z,x)	if(x)return z;
#define return_if(x)		if(x)return;
#define continue_if(x)		if(x){continue;}
#define do_if(x,y,a)		if(x==y){a;return true;}else{return false;}

namespace BetaGUI2 {
////////////////////////////////////////////////////////////////////////////////////
WidgetStyle::WidgetStyle(const Ogre::String &style) {
	mBackground = "bgui.default"; centerText = false; mColour = Ogre::ColourValue::White; mDecalSize = 0;
	mHeight = 16; mFontSize = 0; mFontFace = ""; if (style.length()>=0) fromStyle(style);}
////////////////////////////////////////////////////////////////////////////////////
#if BETAGUI_USES_OIS
bool GUI::frameEnded(const Ogre::FrameEvent &evt){
	if(mCapturing){mMouse->capture();mKeyboard->capture();}return true;}
////////////////////////////////////////////////////////////////////////////////////
GUI::GUI(Ogre::RenderWindow* w, OIS::InputManager* im) : mInputManager(im), mCapturing(true), mOver(false),
#else
GUI::GUI(Ogre::RenderWindow* w) :
#endif
	mWindowToDestroy(0), mMousePointer(0), mWindowCount(0), mButtonCount(0), 
	mTextInputCount(0), mRenderWindowWidth(w->getWidth()), mRenderWindowHeight(w->getHeight()),
	mActiveWindow(0), mWindowRatio(Ogre::Real(mRenderWindowWidth) / Ogre::Real(mRenderWindowHeight)) {
	mOverlay = Ogre::OverlayManager::getSingleton().create("BetaGUI2"); mOverlay->show();
#if BETAGUI_USES_OIS
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard,true));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse,true));
	mMouse->setEventCallback(this);mKeyboard->setEventCallback(this);
	Ogre::Root::getSingleton().addFrameListener(this);
#endif
}///////////////////////////////////////////////////////////////////////////////////
GUI::~GUI() {
#if BETAGUI_USES_OIS
	Ogre::Root::getSingleton().removeFrameListener(this);
	mInputManager->destroyInputObject(mMouse);mInputManager->destroyInputObject(mKeyboard);
#endif
	for(unsigned int i=0;i < mWindow.size();i++) delete mWindow[i];
	for (std::map<Ogre::String, Style*>::iterator it = mStyle.begin();it!=mStyle.end();it++) delete (*it).second;
	mStyle.clear(); mWindow.clear();}
////////////////////////////////////////////////////////////////////////////////////
bool GUI::injectRelMouse(int x, int y, bool LMB) {
	return injectMouse(mMousePointer->getLeft() + x, mMousePointer->getTop() + y, LMB);}
////////////////////////////////////////////////////////////////////////////////////
bool GUI::injectMouse(unsigned int x, unsigned int y, bool LMB) {	
	if (x < 0 || x > mRenderWindowWidth) x = lastKnownMouseX = mMousePointer->getLeft();
	if (y < 0 || y > mRenderWindowHeight) y = lastKnownMouseY = mMousePointer->getTop();
	if (mMousePointer)mMousePointer->setPosition(x,y);
	for(std::vector<Window*>::iterator i=mWindow.begin();i!=mWindow.end();i++) {
		if (mWindowToDestroy==(*i)){	delete (*i);
										mWindow.erase(i);
										mWindowToDestroy=0;
										return false;}
		if((*i)->check(x,y,LMB))return true;}
	return false;}
////////////////////////////////////////////////////////////////////////////////////
Ogre::OverlayContainer* GUI::__getOverlay(const Ogre::String& name, const Ogre::String& type,int x,int y, int w, int h) {
	Ogre::OverlayContainer* oc; try { oc = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().getOverlayElement(name));
	}catch(...) {oc = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement(type, name));}
	oc->setMetricsMode(Ogre::GMM_PIXELS);
	return_this_if(oc, x==0&&y==0&&w==0)
	oc->setPosition(x,y); oc->setDimensions(w,h); return oc;}
////////////////////////////////////////////////////////////////////////////////////
Ogre::OverlayContainer* GUI::createOverlayFromWidgetStyle(const Ogre::String &name, const Ogre::String &styleName, const Ogre::String &widgetType, int x, int y, const Ogre::String &text) {
	WidgetStyle ws=mStyle[styleName]->mWidgets[widgetType];
	Ogre::OverlayContainer* oc=__getOverlay(name,"Panel", x,y,32,ws.mHeight);
	oc->setMaterialName(ws.mBackground);
	return_this_if(oc,text.length()==0)
	int fontSize=ws.mFontSize?ws.mFontSize:mStyle[styleName]->mFontSize;
	Ogre::String fontName=ws.mFontFace.length()?ws.mFontFace:mStyle[styleName]->mFontFace;
	Ogre::OverlayContainer* caption = __getOverlay(name + "-caption", "TextArea", 0,((ws.mHeight * 0.5f) - ((float) fontSize) * 0.5f) * mWindowRatio, 32, fontSize);
	oc->addChild(caption);
	caption->setCaption(text);caption->setParameter("font_name", fontName);caption->setParameter("char_height", Ogre::StringConverter::toString(fontSize));
	Ogre::Font* f = dynamic_cast<Ogre::Font*>(Ogre::FontManager::getSingletonPtr()->getByName(fontName).get());
	f->setTrueTypeSize(fontSize);	float textWidth = 0; Ogre::Real sp = f->getGlyphAspectRatio(0x0030) * fontSize;
	for(unsigned int i=0; i < text.length();i++){	
		if (text[i] == 0x0020) textWidth += sp;
		else textWidth += f->getGlyphAspectRatio(text[i]) * fontSize;}
	oc->setWidth(Ogre::Math::Ceil(textWidth * 1.25f));	caption->setWidth(textWidth); caption->setHeight(fontSize);
	if (ws.centerText) caption->setLeft(textWidth * 0.125f);
	caption->setColour(ws.mColour);
	return oc;
}
////////////////////////////////////////////////////////////////////////////////////
Ogre::OverlayContainer* GUI::createOverlayFromWindowStyle(const Ogre::String &name, const Ogre::String &styleName, int x, int y, int w, int h) {
	Ogre::OverlayContainer* oc=__getOverlay(name);oc->setPosition(x,y);oc->setDimensions(w,h);
	if (mStyle[styleName]->mWindow_Background.length())oc->setMaterialName(mStyle[styleName]->mWindow_Background);
	return oc;}
////////////////////////////////////////////////////////////////////////////////////
void GUI::__updateCenteredOverlay(Ogre::OverlayContainer* overlay) {
	Ogre::OverlayContainer* caption = static_cast<Ogre::OverlayContainer*>(overlay->getChild(overlay->getName()+"-caption"));
	caption->setPosition((overlay->getWidth()*0.5f)-(caption->getWidth()*0.5f),2+(overlay->getHeight()*0.5f)-(caption->getHeight()*0.5f));}
////////////////////////////////////////////////////////////////////////////////////
Ogre::OverlayContainer* GUI::createMousePointer(const Ogre::String& m, unsigned int w, unsigned int h) {
	Ogre::Overlay* o=Ogre::OverlayManager::getSingleton().create("BetaGUI2.MP");
	mMousePointer=__getOverlay("BetaGUI2.MousePointer","Panel",mRenderWindowWidth*0.5f,mRenderWindowHeight*0.5f,w,h);
	mMousePointer->setMaterialName(m);
	o->setZOrder(649);o->add2D(mMousePointer);o->show();mMousePointer->show();
	return mMousePointer;}
////////////////////////////////////////////////////////////////////////////////////
Style*	GUI::createStyle(const Ogre::String& name){		mStyle[name]=new Style();return mStyle[name];}
////////////////////////////////////////////////////////////////////////////////////
Window*	 GUI::createWindow(int x,int y,unsigned int w, unsigned int h, const Ogre::String& s, WindowFeatureType t, const Ogre::String& c, GUIListener* l) {
	Window* window=new BetaGUI2::Window(x,y,w,h,s,t,c,l,this);mWindow.push_back(window);return window;}
////////////////////////////////////////////////////////////////////////////////////
void GUI::destroyWindow(Window *window) {				mWindowToDestroy=window;}
////////////////////////////////////////////////////////////////////////////////////
void GUI::hideAll(){
	for (unsigned int i=0;i < mWindow.size();i++)	mWindow[i]->hide();}
////////////////////////////////////////////////////////////////////////////////////
void GUI::setActiveWindow(Window* w){w->setToActive();
	for (unsigned int i=0;i < mWindow.size();i++){if (mWindow[i]!=w)mWindow[i]->setToNormal();}}
////////////////////////////////////////////////////////////////////////////////////
Window::Window(int X, int Y, unsigned int width, unsigned int height, const Ogre::String& style, WindowFeatureType type, const Ogre::String& caption, GUIListener* listener, GUI* gui) 
	: x(X), y(Y), w(width), h(height), mGUI(gui), mTitlebarButton(0), mResizeButton(0), mActiveTextInput(0), mActiveButton(0), mStyle(style), mMode(WM_NONE), mListener(listener),
	  mCursorX(x),mCursorY(y), mCursorLineHeight(1), mCursorAutoSize(false), mCursorLeftRight(true), mCursorCenter(false) {
	mSheet = Ogre::OverlayManager::getSingleton().create("BG" + Ogre::StringConverter::toString(gui->mWindowCount++)); mSheet->show();
	mSheet->setZOrder(z = 100 + mGUI->mWindowCount);
	mOverlay = gui->createOverlayFromWindowStyle(mSheet->getName() + "/window",style,x,y,w,h);
	if (mGUI->getStyle(style)->mWindow_Border == true) {
		mOverlay->setMaterialName("betagui.transparent");
		bl = gui->__getOverlay(mSheet->getName() + "/bl", "Panel",0,0,1,height-1);
		br = gui->__getOverlay(mSheet->getName() + "/br", "Panel",width-1,0,1,height-1);
		bb = gui->__getOverlay(mSheet->getName() + "/bb", "Panel",0,height-1,width,1);
		mOverlay->addChild(bl);mOverlay->addChild(br);mOverlay->addChild(bb);
		bl->setMaterialName(mGUI->getStyle(style)->mWindow_Background + ".border");
		br->setMaterialName(mGUI->getStyle(style)->mWindow_Background + ".border");
		bb->setMaterialName(mGUI->getStyle(style)->mWindow_Background + ".border");
		bl->show();br->show();bb->show();
		if (type==WFT_MOVE||type==WFT_RESIZE_AND_MOVE)
			mWidgetContainer = gui->__getOverlay(mSheet->getName() + "/wc", "Panel",1,22,width-2, height - 22 - 2);
		else
			mWidgetContainer = gui->__getOverlay(mSheet->getName() + "/wc", "Panel",1,0,width,height-2);
		mWidgetContainer->setMaterialName(mGUI->getStyle(style)->mWindow_Background);
		mOverlay->addChild(mWidgetContainer);
		mWidgetContainer->show();
	}
	else {
		mWidgetContainer = gui->__getOverlay(mSheet->getName() + "/wc", "Panel",1,0,width,height);
		if (mGUI->getStyle(style)->mWindow_Background.length())
		mWidgetContainer->setMaterialName(mGUI->getStyle(style)->mWindow_Background);
	}
	mSheet->add2D(mOverlay); mOverlay->show();

	if (type == WFT_RESIZE || type == WFT_RESIZE_AND_MOVE) {
//		Callback c;c.t=4;
//		mResizeButton=createButton(Ogre::Vector4(width-16,height-16,16,16),"bgui.button", "",c);
//		mResizeButton = new Button(0,0,mWidth,0, caption, this, WR_RESIZE);
//		mButtons.push_back(mResizeButton);
	}

	if (type==WFT_MOVE||type==WFT_RESIZE_AND_MOVE){		mTitlebarButton=new Button(0,0,w,22,caption, this, WR_DRAG);
														mButtons.push_back(mTitlebarButton);}

}
////////////////////////////////////////////////////////////////////////////////////
Window::~Window() {
	for(unsigned int i=0;i<mButtons.size();i++)			delete mButtons[i];
	for(unsigned int i=0;i<mTextInputs.size();i++)		delete mTextInputs[i];
	mGUI->mOverlay->remove2D(mOverlay);}
////////////////////////////////////////////////////////////////////////////////////
void Window::setToNormal() {
	if (mGUI->getStyle(mStyle)->mWindow_Background.length() != 0)
		mWidgetContainer->setMaterialName(mGUI->getStyle(mStyle)->mWindow_Background);
	toZ();
	if (mTitlebarButton)
		mTitlebarButton->setCaption("Normal");
}
////////////////////////////////////////////////////////////////////////////////////
void Window::setToActive() {
	if (mGUI->getStyle(mStyle)->mWindow_ActiveBackground.length() != 0)
		mWidgetContainer->setMaterialName(mGUI->getStyle(mStyle)->mWindow_ActiveBackground);
	toFront();
	if (mTitlebarButton)
		mTitlebarButton->setCaption("Active");
}
////////////////////////////////////////////////////////////////////////////////////
void Window::toFront(){mSheet->setZOrder(z = 100 + mGUI->mWindowCount + 10);}
void Window::toZ(){mSheet->setZOrder(z);}
void Window::setZ(int Z){z=Z;toZ();}
////////////////////////////////////////////////////////////////////////////////////
void Window::setPosition(unsigned int X,unsigned int Y) {		mOverlay->setPosition(x=X,y=Y);}
////////////////////////////////////////////////////////////////////////////////////
Ogre::Vector2 Window::getPosition() {	return Ogre::Vector2(x,y);		}
Ogre::Vector2 Window::getSize()		{	return Ogre::Vector2(w,h);		}
////////////////////////////////////////////////////////////////////////////////////
void Window::setSize(unsigned int X,unsigned int Y) {mOverlay->setDimensions(w=X,h=Y);
	if (mResizeButton)			mResizeButton->mOverlay->setPosition(mResizeButton->x=w-16,mResizeButton->y=h-16);
	if (mTitlebarButton)		mTitlebarButton->mOverlay->setWidth(mTitlebarButton->w=w);}
////////////////////////////////////////////////////////////////////////////////////
Window& Window::button(std::string caption, BetaGUI2::ButtonMethodPtr bmptr) {
	int tx = (mCursorLeftRight ? mCursorX : -int(mCursorX));
	BetaGUI2::Button*T = createButton(tx,mCursorY,caption, bmptr);
	if (mCursorCenter) {
		T->setX((getSize().x * 0.5f) - (T->getWidth() * 0.5f) - mCursorX);mCursorCenter = false;}
	_AdjustCursor(mCursorX + T->getWidth(),T->getHeight());
	return *this;
}
////////////////////////////////////////////////////////////////////////////////////
Window& Window::input(std::string value, unsigned int length, NxReal widgetLength) {
	if (widgetLength=0)	widgetLength = length * 15;
	int tx = (mCursorLeftRight ? mCursorX : -int(mCursorX));
	BetaGUI2::TextInput* T = createTextInput(tx,mCursorY,widgetLength, value, length);
	_AdjustCursor(mCursorX + (widgetLength * 3),20);
	return *this;
}
////////////////////////////////////////////////////////////////////////////////////
Window& Window::text(std::string caption){
	Ogre::OverlayContainer* T = createStaticText((mCursorLeftRight?mCursorX:-int(mCursorX)),mCursorY,caption);
	_AdjustCursor(mCursorX+T->getWidth(),T->getHeight());return *this; 
}
////////////////////////////////////////////////////////////////////////////////////
void Window::_AdjustCursor(unsigned int nx, unsigned int nh) {
	if (nh > (unsigned) mCursorLineHeight) mCursorLineHeight = nh + 1;
	if (nx > (unsigned int)(getSize().x)){
		if (mCursorAutoSize){setSize(nx, getSize().y);}
		lineBreak();return;}
	mCursorX = nx + 1;
}
////////////////////////////////////////////////////////////////////////////////////
Button* Window::createButton(int x, int y, const Ogre::String& caption, ButtonMethodPtr method) {
	Button *button = new Button(x,y, caption, method, this);mButtons.push_back(button);return button;}
////////////////////////////////////////////////////////////////////////////////////
Button::Button(int X, int Y, const Ogre::String& caption, ButtonMethodPtr method, Window *parent)
 :x(X),y(Y),mMethod(method),mParent(parent),mOverlay(0),mCaptionOverlay(0),mDecalLeft(0),mDecalRight(0),mRole(WR_NONE){
	
	std::string buttonStyle = "button";
	std::string buttonCaption = caption;
	if (caption.substr(0,1)=="+"){	buttonCaption = caption.substr(1, caption.length() - 1);
									parent->mOkayButton = this;buttonStyle = "okay-button";}

	if (caption.substr(0,1)=="-"){	buttonCaption = caption.substr(1, caption.length() - 1);
									parent->mCancelButton = this;buttonStyle="cancel-button";}

	int decalWidth = parent->mGUI->mStyle[parent->mStyle]->mWidgets["button"].mDecalSize;
	Ogre::String overlayID = parent->mOverlay->getName() + "/button;" + Ogre::StringConverter::toString(parent->mGUI->mButtonCount++);
	mOverlay = mParent->mGUI->createOverlayFromWidgetStyle(overlayID, parent->mStyle, buttonStyle, x + decalWidth, y, buttonCaption);
	if (caption.length()) mCaptionOverlay = static_cast<Ogre::OverlayContainer*>(mOverlay->getChild(mOverlay->getName() + "-caption"));
	//mCaptionOverlay->setMaterialName("betagui.transparent");
	h = mOverlay->getHeight();
	parent->mOverlay->addChild(mOverlay);mOverlay->show();

	mBackground_Normal = mOverlay->getMaterialName();
	mBackground_Active = mBackground_Normal + "-active";
	
	if (decalWidth != 0){			mOverlay->addChild(mDecalLeft = parent->mGUI->__getOverlay(mOverlay->getName() + ",L", "Panel", -decalWidth,0, decalWidth, mOverlay->getHeight()));
									mDecalLeft->setMaterialName(mOverlay->getMaterialName() + "L");
									mOverlay->addChild(mDecalRight = parent->mGUI->__getOverlay(mOverlay->getName() + ",R", "Panel", mOverlay->getWidth(),0, decalWidth, mOverlay->getHeight())); 
									mDecalRight->setMaterialName(mOverlay->getMaterialName() + "R");
									w = mOverlay->getWidth() + mDecalLeft->getWidth();}

	else{							w = mOverlay->getWidth();}

	return_if(!caption.length())

	if(x<0){						x = parent->getSize().x - w - Ogre::Math::Abs(x);
									mOverlay->setLeft(x);}

	if(y<0){						y = parent->getSize().y - h - Ogre::Math::Abs(y);
									mOverlay->setTop(y);}

}
////////////////////////////////////////////////////////////////////////////////////
Button::Button(int X, int Y, int W, int H, const Ogre::String& caption, Window *parent, WindowRole role)
	:x(X),y(Y),w(W),h(H),mParent(parent),mOverlay(0),mCaptionOverlay(0),mDecalLeft(0),mDecalRight(0),mRole(role){

	Ogre::String overlayID = parent->mOverlay->getName() + "/titlebar";
	parent->mGUI->mButtonCount++;

	if(mRole==WR_DRAG){			mOverlay = mParent->mGUI->createOverlayFromWidgetStyle(overlayID,parent->mStyle,"titlebar",x,y,caption);
								parent->mOverlay->addChild(mOverlay);
								mOverlay->setWidth(w=W);
								h=mOverlay->getHeight();
								parent->mGUI->__updateCenteredOverlay(mOverlay);
								mOverlay->show();
								mBackground_Normal = mOverlay->getMaterialName();
								mBackground_Active = mBackground_Normal;
								return;}

}
////////////////////////////////////////////////////////////////////////////////////
void Button::setX(int X) { mOverlay->setLeft(x = X);}
////////////////////////////////////////////////////////////////////////////////////
void Button::setY(int Y) {	mOverlay->setTop(y = Y);}
////////////////////////////////////////////////////////////////////////////////////
unsigned int Button::getWidth() {
	unsigned int w = (unsigned int)(mOverlay->getWidth());
	return_this_if(w, mDecalLeft == 0);
	w+= mDecalLeft->getWidth() + mDecalRight->getWidth();
	return w;
}
////////////////////////////////////////////////////////////////////////////////////
unsigned int Button::getHeight(){return (unsigned int)(mOverlay->getHeight());}
////////////////////////////////////////////////////////////////////////////////////
void Button::setToActive() {
	mOverlay->setMaterialName(mBackground_Active);
	return_if(mDecalLeft == 0);
	mDecalLeft->setMaterialName(mBackground_Active + "L");
	mDecalRight->setMaterialName(mBackground_Active + "R");
}
////////////////////////////////////////////////////////////////////////////////////
void Button::setToNormal() {
	mOverlay->setMaterialName(mBackground_Normal);
	return_if(mDecalLeft == 0)
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
	Ogre::Font* f = dynamic_cast<Ogre::Font*>(Ogre::FontManager::getSingletonPtr()->getByName(mGUI->mStyle[mStyle]->mFontFace).get());
	int fontSize = mGUI->mStyle[mStyle]->mFontSize;f->setTrueTypeSize(fontSize);
	float textWidth = 0; Ogre::Real sp = f->getGlyphAspectRatio(0x0030) * fontSize;
	for(unsigned int i=0; i < caption.length();i++){	
		if (caption[i] == 0x0020)	textWidth += sp;
		else textWidth += f->getGlyphAspectRatio(caption[i]) * fontSize;}
	text->setWidth(Ogre::Math::Ceil(textWidth));text->setHeight(fontSize);
	if(x<0){						x = getSize().x - text->getWidth() - Ogre::Math::Abs(x);
									text->setLeft(x);}
	if(y<0){						y = getSize().y - text->getHeight() - Ogre::Math::Abs(y);
									text->setTop(y);}
	text->show();mWidgetContainer->addChild(text);
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
void TextInput::setToActive() {
	mOverlay->setMaterialName(mBackground_Active);
	return_if(mDecalLeft == 0)
	mDecalLeft->setMaterialName(mBackground_Active + "L");
	mDecalRight->setMaterialName(mBackground_Active + "R");
}
////////////////////////////////////////////////////////////////////////////////////
void TextInput::setToNormal() {
	mOverlay->setMaterialName(mBackground_Normal);
	return_if(mDecalLeft == 0)
	mDecalLeft->setMaterialName(mBackground_Normal + "L");
	mDecalRight->setMaterialName(mBackground_Normal + "R");
}
////////////////////////////////////////////////////////////////////////////////////
bool Window::checkKey(const Ogre::String& k, unsigned int px, unsigned int py) {
	return_this_if(false, !mOverlay->isVisible());
	return_this_if(false, !(px>=x&&py>=y)||!(px<=x+w&&py<=y+h));
	return_this_if(false, mActiveTextInput == 0);

	mActiveTextInput->mCaptionOverlay->setCaption(mActiveTextInput->mValue+=k);
	return true;
}
////////////////////////////////////////////////////////////////////////////////////
bool Window::checkSpecialKey(SpecialKey key,unsigned int px,unsigned int py) {
	return_this_if(false, !mOverlay->isVisible());
	return_this_if(false, !(px>=x&&py>=y)||!(px<=x+w&&py<=y+h));
	if (key == SK_ENTER && mOkayButton && mActiveTextInput == 0)
		(mListener->*mOkayButton->mMethod)(mOkayButton, FS_SELECT_OFF);
	if (key == SK_ENTER && mActiveTextInput) {
		mActiveTextInput->setToNormal();mActiveTextInput=0;
	}
	if (key == SK_ESCAPE && mCancelButton && mActiveTextInput == 0) {
		(mListener->*mCancelButton->mMethod)(mCancelButton, FS_SELECT_OFF);
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////////
bool Window::check(unsigned int px, unsigned int py, bool LMB) {
	
	if (!mOverlay->isVisible())	return false;
	
	// Check if we aren't the active window
	if (mGUI->mActiveWindow != this && LMB) {
		if ((px >= x && py >= y) && (px <= x + w && py <= y + h)){
			if (mGUI->mActiveWindow)
				mGUI->mActiveWindow->setToNormal();
			mGUI->mActiveWindow = this;
			mGUI->mActiveWindow->setToActive();
		}
	}


	// Mode is dragging.
	if (mMode == WM_DRAG) {
		if (LMB){	setPosition(px + dx, py + dy);
		}else		mMode = WM_NONE;
		return true;}

	// LMB_DOWN
	//		If LMB is off, and we have an activebutton. execute method and WM_NONE
	if (mMode == WM_LMB_DOWN){
		if (!LMB && mActiveButton){
			(mListener->*mActiveButton->mMethod)(mActiveButton, FS_SELECT_OFF);
			mActiveButton->setToNormal();
			mActiveButton = 0;
			mMode = WM_NONE;
			return true;
		}
	}


	// Is the mouse pointer inside the area of the window.
	//	No? Clear active button, set to normal and return false.
	if (!(px >= x && py >= y) || !(px <= x + w && py <= y + h)){
		if (mActiveButton){	mActiveButton->setToNormal();
		mActiveButton = 0;mMode = WM_NONE;}
	return false;}

	// Is there an active button.
	if (mActiveButton) {
		if (mActiveButton->out(px,py,x,y)) {
			mActiveButton->setToNormal();
			mActiveButton = 0;
			mMode = WM_NONE;
		}
	}

	// For each button. Check if mouse pointer is in it.
	//		Yes: 
	//			- Check if there is an active button
	//				Yes: Set to Normal and Clear
	//			- Assign button to active button and set to active.
	for (unsigned int i=0;i < mButtons.size();i++) {

		if (!mButtons[i]->out(px,py,x,y)) {
			if (LMB) {
				if (mActiveButton) {
					mActiveButton->setToNormal();
					mActiveButton = 0;
				}

				if(mButtons[i]->mRole==WR_NONE) {
					mMode = WM_LMB_DOWN;
					mActiveButton = mButtons[i];
					mActiveButton->setToActive();
				}
				else if(mButtons[i]->mRole==WR_DRAG) {
					mMode = WM_DRAG;
					dx = (int) x - px;	dy = (int) y - py;
				}
				return true;
			}
		}
	}

	return true;
#if 0
	if (!mOverlay->isVisible())	return false;
	if (mMode == WM_DRAG) {
		if (LMB){	setPosition(px + dx, py + dy);
		}else		mMode = WM_NONE;
		return true;}
	if (mMode == WM_LMB_DOWN){
			if (!LMB && mActiveButton){
						(mListener->*mActiveButton->mMethod)(mActiveButton, FS_SELECT_OFF);
						mMode = WM_NONE;}
			else{return false;}}
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

		}
		else {
			mActiveButton->setToNormal();
		}

		if(mActiveTextInput && LMB) {
			mActiveTextInput->setToNormal();
			mActiveTextInput=0;
		}

	}

	if (mActiveTextInput) {
		if (LMB && !mActiveTextInput->in(px,py,x,y)) {
			mActiveTextInput->setToNormal();
			mActiveTextInput=0;
			std::cout << "Text Input cleared: " << mActiveTextInput << std::endl;
			return true;
		}
		return true;
	}


	for (unsigned int i=0;i<mTextInputs.size();i++) {
		continue_if(mTextInputs[i]->in(px,py,x,y));
		std::cout << "New Text Input: " << mActiveTextInput << std::endl;
		mActiveTextInput=mTextInputs[i];
		mActiveTextInput->setToActive();
		return true;
	}
	
	return true;
#endif
}
////////////////////////////////////////////////////////////////////////////////////
void Style::addStyle(Ogre::String widgetName, const Ogre::String& style) {
	std::transform(widgetName.begin(),widgetName.end(),widgetName.begin(),tolower);
	mWidgets[widgetName]=WidgetStyle(style);}
////////////////////////////////////////////////////////////////////////////////////
void WidgetStyle::fromStyle(const Ogre::String& p) {
	std::string delims=" \t\r";
	std::vector<std::string>ds=Ogre::StringUtil::split(p,";");std::pair<std::string,std::string> d; size_t dp;
	for(std::vector<std::string>::iterator it = ds.begin();it != ds.end();++it) {
		dp = (*it).find_first_of(":");
		d.first = (*it).substr(0,dp);d.second = (*it).substr(dp+1, (*it).length() - dp - 1);
		d.first.erase(d.first.find_last_not_of(delims)+1);d.first.erase(0,d.first.find_first_not_of(delims));
		d.second.erase(d.second.find_last_not_of(delims)+1);d.second.erase(0,d.second.find_first_not_of(delims));
		std::transform(d.first.begin(),d.first.end(),d.first.begin(),tolower);
		continue_if(Style::_set("background", d, mBackground))
		continue_if(Style::_set("background", d, mBackground))
		continue_if(Style::_set("font-face", d, mFontFace))
		continue_if(Style::_set("font-size", d, mFontSize))
		continue_if(Style::_set("decal-size", d, mDecalSize))
		continue_if(Style::_set("height", d, mHeight))
		continue_if(Style::_set("colour", d, mColour))
		continue_if(Style::_set("color", d, mColour))
		if (d.first == "text-align"){
			std::transform(d.second.begin(),d.second.end(),d.second.begin(),tolower);
			if(d.second=="centre"||d.second=="center"){centerText=true;}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////
bool Style::_set(const Ogre::String& i, std::pair<Ogre::String,Ogre::String>& d , unsigned int& t) {
	do_if(d.first,i,t=Ogre::StringConverter::parseUnsignedInt(d.second))}
bool Style::_set(const Ogre::String& i, std::pair<Ogre::String,Ogre::String>& d , int& t) {
	do_if(d.first,i,t=Ogre::StringConverter::parseInt(d.second))}
bool Style::_set(const Ogre::String& i, std::pair<Ogre::String,Ogre::String>& d, bool& t) {
	do_if(d.first,i,t=Ogre::StringConverter::parseBool(d.second))}
bool Style::_set(const Ogre::String& i , std::pair<Ogre::String,Ogre::String>& d, Ogre::Real& t) {
	do_if(d.first,i,t=Ogre::StringConverter::parseReal(d.second))}
bool Style::_set(const Ogre::String& i, std::pair<Ogre::String,Ogre::String>& d, Ogre::String& t) {
	do_if(d.first,i,t=d.second)}
bool Style::_set(const Ogre::String& i, std::pair<Ogre::String,Ogre::String>& d, Ogre::ColourValue& t) {
	do_if(d.first,i,t=Ogre::StringConverter::parseColourValue(d.second))}
////////////////////////////////////////////////////////////////////////////////////
bool GUI::injectKey(const Ogre::String& key) {
	for(std::vector<Window*>::iterator i=mWindow.begin();i!=mWindow.end();i++) {
		if((*i)->checkKey(key,lastKnownMouseX,lastKnownMouseY))return true;}
	return false;
}
////////////////////////////////////////////////////////////////////////////////////
void GUI::injectSpecialKey(SpecialKey key) {
	for(unsigned int i=0;i<mWindow.size();i++) {
		if(mWindow[i]->checkSpecialKey(key,lastKnownMouseX,lastKnownMouseY)){return;}
	}
}
////////////////////////////////////////////////////////////////////////////////////
#if BETAGUI_USES_OIS
bool GUI::mouseMoved(const OIS::MouseEvent &e){mOver = injectRelMouse(e.state.X.rel,e.state.Y.rel, e.state.buttonDown(OIS::MB_Left));return false;}
bool GUI::mousePressed(const OIS::MouseEvent &e,OIS::MouseButtonID i){mOver = injectRelMouse(e.state.X.rel,e.state.Y.rel, e.state.buttonDown(OIS::MB_Left));return false;}
bool GUI::mouseReleased(const OIS::MouseEvent &e,OIS::MouseButtonID i){mOver = injectRelMouse(e.state.X.rel,e.state.Y.rel, e.state.buttonDown(OIS::MB_Left));return false;}
////////////////////////////////////////////////////////////////////////////////////
bool GUI::keyPressed(const OIS::KeyEvent &e) {
	if (e.key == SK_BACKSPACE || e.key == SK_ENTER || e.key == SK_ESCAPE || e.key == SK_TAB) {
		injectSpecialKey((SpecialKey) e.key); return false;
	}
	std::string k;
	char c;
	c = static_cast<char>(e.text);
	k.append(k);
	injectKey(k);
	return false; 
}

bool GUI::keyReleased(const OIS::KeyEvent &e) {return false;}
#endif
////////////////////////////////////////////////////////////////////////////////////
#undef return_this_if
#undef return_if
#undef continue_if
#undef do_if
} // End of Betajaen's GUI. Normal programming can resume now. 
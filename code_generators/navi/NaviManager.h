/*
	This file is part of Navi, a library that allows developers to embed movable 
	'Navis' (Dynamic, HTML/JS/CSS-Driven GUI Overlays) within an Ogre3D application.

	Copyright (C) 2007 Adam J. Simmons
	http://www.agelessanime.com/Navi/

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __NaviManager_H__
#define __NaviManager_H__
#if _MSC_VER > 1000
#pragma once
#endif

#include "NaviPlatform.h"
#include "NaviData.h"
#include "NaviEventListener.h"
#include "NaviMouse.h"
//#include "NaviDelegate.h"
#include "NaviUtilities.h"
#include "NaviSingleton.h"
#include <OgrePanelOverlayElement.h>

namespace NaviLibrary
{
	struct NaviCompare;

	/**
	* Enumerates relative positions. Used by NaviManager::NaviPosition
	*/
	enum RelativePosition
	{
		Left,
		TopLeft,
		TopCenter,
		TopRight,
		Right,
		BottomRight,
		BottomCenter,
		BottomLeft,
		Center
	};

	/**
	* An object that holds position-data for a Navi. Used by NaviManager::createNavi and NaviManager::setNaviPosition.
	*/
	class _NaviExport NaviPosition
	{
		bool usingRelative;
		union {
			struct { RelativePosition position; short x; short y; } rel;
			struct { short left; short top; } abs;
		} data;

		friend class Navi;
		NaviPosition();
	public:
		/**
		* Creates a relatively-positioned NaviPosition object.
		*
		* @param	relPosition		The position of the Navi in relation to the Render Window
		*
		* @param	offsetLeft	How many pixels from the left to offset the Navi from the relative position.
		*
		* @param	offsetTop	How many pixels from the top to offset the Navi from the relative position.
		*/
		NaviPosition(const RelativePosition &relPosition, short offsetLeft = 0, short offsetTop = 0);

		/**
		* Creates an absolutely-positioned NaviPosition object.
		*
		* @param	absoluteLeft	The number of pixels from the left of the Render Window.
		*
		* @param	absoluteTop		The number of pixels from the top of the Render Window.
		*/
		NaviPosition(short absoluteLeft, short absoluteTop);
	};

	/**
	* Enumerates internal mouse button IDs. Used by NaviManager::injectMouseDown, NaviManager::injectMouseUp
	*/
	enum MouseButtonID
	{
		LeftMouseButton = 0,
		RightMouseButton, 
		MiddleMouseButton
	};
 
	/**
	* Supreme dictator and Singleton: NaviManager
	*
	* The class you will need to go to for all your Navi-related needs.
	*/
	class _NaviExport NaviManager : public Singleton<NaviManager>
	{
		friend class Navi; // Our very close friend <3
		friend void NaviUtilities::translateLocalProtocols(std::string &strToTranslate);

		bool startedUp;
		std::string localNaviDirectory;
		std::map<std::string,Navi*> activeNavis;
		std::vector<Navi*> boundaryIgnorers;
		Navi* focusedNavi;
		int hiddenWindowID;
		std::map<std::string,Navi*>::iterator iter;
		std::vector<Navi*>::iterator bIter;
		Ogre::RenderWindow* renderWindow;
		int mouseXPos, mouseYPos;
		bool mouseButtonRDown;
		unsigned short zOrderCounter;
		NaviMouse* mouse;

		void focusNavi(int x, int y, Navi* selection = 0);
		const std::vector<Navi*>& getNavisAtPoint(int x, int y);
		const std::vector<Navi*>& getNavis();
	public:
		/**
		* Creates the NaviManager and loads the internal LLMozLib library.
		*
		* @param	_renderWindow	The Ogre::RenderWindow to render Navis to
		*
		* @param	_localNaviDirectory		By default, "local://" means something like: 
		*									"C:\MyApplicationDirectory\NaviLocal\" but by changing
		*									this parameter, you can change "NaviLocal" to something else.
		*
		* @throws	Ogre::Exception::ERR_INTERNAL_ERROR		Throws this when LLMozLib fails initialization
		*/
		NaviManager(Ogre::RenderWindow* _renderWindow, const std::string &_localNaviDirectory = "NaviLocal");

		~NaviManager();

		/**
		* Gets the NaviManager Singleton.
		*
		* @return	A reference to the NaviManager Singleton.
		*
		* @throws	Ogre::Exception::ERR_RT_ASSERTION_FAILED	Throws this if NaviManager has not been new'd yet.
		*/
		static NaviManager& Get();

		/**
		* Gets the NaviManager Singleton as a pointer.
		*
		* @return	A pointer to the NaviManager Singleton.
		*
		* @throws	Ogre::Exception::ERR_RT_ASSERTION_FAILED	Throws this if NaviManager has not been new'd yet.
		*/
		static NaviManager* GetPointer();

		/**
		* Starts up the NaviMouse singleton and returns a pointer to it.
		*
		* @param	visible		Whether or not the NaviMouse is visible. Use NaviMouse::show()/hide() later.
		*/
		NaviMouse* StartupMouse(bool visible = true);

		/**
		* Returns the NaviMouse singleton.
		*
		* @throws	Ogre::Exception::ERR_RT_ASSERTION_FAILED	Throws this if StartupMouse() is not called prior.
		*/
		NaviMouse* getMouse();

		/**
		* Gives each active Navi a chance to update, each may or may not update their internal textures
		* based on various conditions.
		*/
		void Update();

		/**
		* Destroys any active Navis and shuts down the LLMozLib library.
		*/
		void Shutdown();

		/**
		* Creates a Navi. (You MUST call Startup() before this.)
		*
		* @param	naviName	The name of the Navi, used to refer to a specific Navi in subsequent calls.
		*
		* @param	homepage	The default starting page for a Navi. You may use local:// here to refer to
		*						the local Navi directory (See NaviManager::Startup)
		*
		* @param	naviPosition	The unified position (either relative or absolute) of a Navi.
		*							See NaviManager::NaviPosition for more information.
		*
		* @param	width	The width of the Navi. MUST be a power of 2 (e.g. 128, 256, 512, 1024)
		*					Technically you may be able to use a number that is divisible by 16 but
		*					please be mindful of the 2^n texture size limitation of certain video cards
		*
		* @param	height	The height of the Navi. MUST be a power of 2 (e.g. 128, 256, 512, 1024)
		*					Technically you may be able to use a number that is divisible by 16 but
		*					please be mindful of the 2^n texture size limitation of certain video cards
		*
		* @param	isMovable	Whether or not this absolutely positioned Navi is movable (right-click-drag to move)
		*
		* @param	isVisible	Whether or not this Navi is visible upon creation. Use NaviManager::showNavi to display
		*						this Navi later.
		*
		* @param	maxUpdatesPerSec	This parameter limits the number of updates per second to a specific 
		*								integer. To use no limiting, leave this parameter as '0'. This limiting is
		*								useful if, for example, you had some very hyperactive Javascript
		*								that animated a moving ball. Without update limiting, Navi will try to update
		*								itself at every possible moment: this is great for very smooth animation but
		*								bad for the overall framerate.
		*
		* @param	forceMaxUpdate		Navi normally only updates when the page has changed, to override this functionality
		*								set this parameter to 'True' to make Navi 'force update' using the value of the 
		*								parameter 'maxUpdatesPerSec'. This is useful as a work-around for rendering embedded 
		*								Flash applications. Note: if 'maxUpdatesPerSec' is 0, Navi will try to 'force update'
		*								every single chance it gets (not recommended).
		*
		* @param	zOrder		Sets the starting Z-Order for this Navi; Navis with higher Z-Orders will be on top of other
		*						Navis. To auto-increment this value for every successive Navi, leave this parameter as '0'.
		*
		* @param	opacity		Sets the starting Opacity of the Navi.
		*						Ex. 1.0000 = Fully opaque
		*						Ex. 0.5000 = Half opaque
		*						Ex. 0.0000 = Totally transparent
		*
		* @throws	Ogre::Exception::ERR_RT_ASSERTION_FAILED	Throws this if NaviManager::Startup is not called prior to this.
		*/
		void createNavi(const std::string &naviName, const std::string &homepage, const NaviPosition &naviPosition,
			unsigned short width, unsigned short height, bool isMovable = true, bool isVisible = true, unsigned int maxUpdatesPerSec = 48, bool forceMaxUpdate = false, 
			unsigned short zOrder = 0, float opacity = 1.0);

		/**
		* Creates a NaviMaterial. (You MUST call Startup() before this.) NaviMaterials are just like Navis except that they lack
		* a movable overlay element. Instead, you handle the material and apply it to anything you like. Mouse input for NaviMaterials
		* must be injected using the 'injectNaviMaterial...(...)' API calls.
		*
		* @param	naviName	The name of the NaviMaterial, used to refer to this specific Navi in subsequent calls.
		*
		* @param	homepage	The default starting page for a Navi. You may use local:// here to refer to
		*						the local Navi directory (See NaviManager::Startup)
		*
		* @param	width	The width of the NaviMaterial. MUST be a power of 2 (e.g. 128, 256, 512, 1024)
		*					Technically you may be able to use a number that is divisible by 16 but
		*					please be mindful of the 2^n texture size limitation of certain video cards
		*
		* @param	height	The height of the NaviMaterial. MUST be a power of 2 (e.g. 128, 256, 512, 1024)
		*					Technically you may be able to use a number that is divisible by 16 but
		*					please be mindful of the 2^n texture size limitation of certain video cards
		*
		* @param	isVisible	Whether or not this NaviMaterial is visible or not (0% opacity, non-updated). Use 
		*						NaviManager::showNavi to display this NaviMaterial later.
		*
		* @param	maxUpdatesPerSec	This parameter limits the number of updates per second to a specific 
		*								integer. To use no limiting, leave this parameter as '0'. This limiting is
		*								useful if, for example, you had some very hyperactive Javascript
		*								that animated a moving ball. Without update limiting, Navi will try to update
		*								itself at every possible moment: this is great for very smooth animation but
		*								bad for the overall framerate.
		*
		* @param	forceMaxUpdate		Navi normally only updates when the page has changed, to override this functionality
		*								set this parameter to 'True' to make Navi 'force update' using the value of the 
		*								parameter 'maxUpdatesPerSec'. This is useful as a work-around for rendering embedded 
		*								Flash applications. Note: if 'maxUpdatesPerSec' is 0, Navi will try to 'force update'
		*								every single chance it gets (not recommended).
		*
		* @param	opacity		Sets the starting Opacity of the Navi.
		*						Ex. 1.0000 = Fully opaque
		*						Ex. 0.5000 = Half opaque
		*						Ex. 0.0000 = Totally transparent
		*
		* @param	texFiltering	The texture filtering to use for this material. If the NaviMaterial is applied to a 3D object,
		*							anisotropic is the best choice, otherwise set this to none for use in other overlays/GUI elements.
		*
		* @throws	Ogre::Exception::ERR_RT_ASSERTION_FAILED	Throws this if NaviManager::Startup is not called prior to this.
		*/
		std::string createNaviMaterial(const std::string &naviName, const std::string &homepage, unsigned short width, unsigned short height, 
			bool isVisible = true, unsigned int maxUpdatesPerSec = 48, bool forceMaxUpdate = false, float opacity = 1.0, Ogre::FilterOptions texFiltering = Ogre::FO_ANISOTROPIC);

		/**
		* Changes the page of the Navi to a supplied URL String.
		* 
		* @param	naviName	The name of the Navi to do this to
		*
		* @param	url		The URL to navigate to. You may use the "local://" protocol here.
		*/
		void navigateNaviTo(const std::string &naviName, const std::string &url);

		/**
		* Changes the page of the Navi to a supplied URL String and sends NaviData to the page.
		* 
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	url		The URL to navigate to. You may use the "local://" protocol here.
		* 
		* @param	naviData	The NaviData to send.
		*/
		void navigateNaviTo(const std::string &naviName, const std::string &url, const NaviData &naviData);

		/**
		* Returns whether or not the page of the Navi can navigate backwards
		*
		* @param	naviName	The name of the Navi to do this to.
		*/
		bool canNavigateBack(const std::string &naviName);

		/**
		* Navigates the page of the Navi backwards, if possible
		*
		* @param	naviName	The name of the Navi to do this to.
		*/
		void navigateNaviBack(const std::string &naviName);

		/**
		* Returns whether or not the page of the Navi can navigate forwards
		*
		* @param	naviName	The name of the Navi to do this to.
		*/
		bool canNavigateForward(const std::string &naviName);

		/**
		* Navigates the page of the Navi forwards, if possible
		*
		* @param	naviName	The name of the Navi to do this to.
		*/
		void navigateNaviForward(const std::string &naviName);

		/**
		* If a Navi's page is loading, stops the load
		*
		* @param	naviName	The name of the Navi to do this to.
		*/
		void navigateNaviStop(const std::string &naviName);

		/**
		* Evaluates a Javascript string in the context of a Navi page and returns the result.
		*
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	script		The Javascript (as a string) to evaluate/execute in the context of the page
		*						that the Navi is currently on.
		*
		* @return	If it succeeds, this will return the result as a string (regardless of internal
		*			Javascript datatype), otherwise this returns an empty string.
		*/
		std::string naviEvaluateJS(const std::string &naviName, const std::string &script);

		/**
		* Destroys a Navi.
		*
		* @param	naviName	The name of the Navi to do this to.
		*/
		void destroyNavi(const std::string &naviName);

		/**
		* Sets the default color to use between changing pages, the default is White if you never call this.
		*
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	red		The Red color value as a float; maximum 1.0, minimum 0.0.
		* @param	green	The Green color value as a float; maximum 1.0, minimum 0.0.
		* @param	blue	The Blue color value as a float; maximum 1.0, minimum 0.0.
		*/
		void setNaviBackgroundColor(const std::string &naviName, float red = 1.0f, float green = 1.0f, float blue = 1.0f);

		/**
		* Sets the default color to use between changing pages, the default is White ("#FFFFFF") if you never call this.
		*
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	hexColor	A hex color string in the format of: "#XXXXXX"
		*/
		void setNaviBackgroundColor(const std::string &naviName, const std::string& hexColor = "#FFFFFF");

		/**
		* Changes the Opacity of a Navi to a provided float.
		*
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	opacity		The Opacity value as a float. 
		*						Fully Opaque = 1.0, Fully Transparent = 0.0.
		*/
		void setNaviOpacity(const std::string &naviName, float opacity = 1.0f);

		/**
		* Sets the filename of the Alpha Mask Image. Navi will use the alpha channel values of the provided image
		* to mask a Navi dynamically.
		*
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	maskFileName	The filename of the Alpha Mask Image. The Alpha Mask Image MUST have a
		*							width greater than or equal to the Navi width and it MUST have a height
		*							greater than or equal to the Navi height. Alpha Mask Images larger than
		*							the Navi will not be stretched, instead Navi will take Alpha values starting
		*							from the Top-Left corner of the Alpha Mask Image. To reset Navi to use no
		*							Alpha Mask Image, simply provide an empty String ("").
		*
		* @param	groupName		The Resource Group to find the Alpha Mask Image filename.
		*
		* @throws	Ogre::Exception::ERR_INVALIDPARAMS	Throws this if the width or height of the Alpha Mask Image is
		*												less than the width or height of the Navi it is applied to.
		*/
		void setNaviMask(const std::string &naviName, const std::string &maskFileName, const std::string &groupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		
		/**
		* Using an alpha-mask/color-keying doesn't just affect the visuals of a Navi; by default, Navis 'ignore'
		* mouse movement/clicks over 'transparent' areas of a Navi (Areas with opacity less than 5%). You may
		* disable this behavior or redefine the 'transparent' threshold of opacity to something else other 
		* than 5%.
		*
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	ignoreTrans		Whether or not Navis should ignore 'transparent' areas when mouse-picking.
		*
		* @param	defineThreshold		Areas with opacity less than this percent will be ignored. Default is 5%.
		*/
		void setNaviIgnoreTransparent(const std::string &naviName, bool ignoreTrans = true, float defineThreshold = 0.05);

		/**
		* Normally, mouse movement is only injected into a specific Navi if the mouse is within the boundaries of
		* a Navi and over an opaque area (not transparent). This behavior may be detrimental to certain Navis, for
		* example an animated 'dock' with floating icons on a transparent background: the mouse-out event would never
		* be invoked on each icon because the Navi only received mouse movement input over opaque areas. Use this function
		* to tell a Navi to always inject mouse movement, regardless of boundaries or transparency.
		*
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	ignoreBounds	Whether or not this Navi should ignore bounds/transparency when injecting mouse movement.
		*
		* @note
		*	The occlusivity of each Navi will still be respected, mouse movement will not be injected if another 
		*	Navi is occluding the Navi you set this on.
		*/
		void setNaviIgnoreBounds(const std::string &naviName, bool ignoreBounds = true);

		/**
		* Using an alpha-mask isn't the only way to achieve transparency of a Navi, you can use color-keying instead or
		* at the same time with alpha-masking to achieve the desired result. Color-keying effectively replaces a certain
		* color on the Navi page with a custom color/opacity.
		*
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	keyColor	The color to replace, as a Hex RGB String (Must be "#XXXXXX" !!). Pass an empty string to
		*						disable color-keying.
		*
		* @param	keyFillOpacity		The opacity of the fill color to replace the key color with, as a percent.
		*
		* @param	keyFillColor	The fill color to replace the key color with, as a Hex RGB String (Must be "#XXXXXX" !!)
		*
		* @param	keyFuzziness	The amount of 'fuzziness' to use when keying out a color. Increase this to additionally key out
		*							colors that are similar to the key color. The relative opacity of each 'fuzzy' color will also
		*							be calculated based on the color distance to the key color. There is some slight overhead when
		*							using a keyFuzziness other than 0.0, it's best to use this with Navis that don't update too often.
		*/
		void setNaviColorKey(const std::string &naviName, const std::string &keyColor, float keyFillOpacity = 0.0, const std::string &keyFillColor = "#000000", float keyFuzziness = 0.0);

		/**
		* Limits the number of times per second a Navi may update.
		*
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	maxUPS		The maximum number of times per second a Navi can update. Set this to '0' to use no update limiting.
		*						If the current Navi is set to 'Force Max Updates', this value is used as the number of updates per
		*						second to actually do.
		*/
		void setMaxUpdatesPerSec(const std::string &naviName, unsigned int maxUPS = 0);

		/**
		* Toggles between auto-updating and force-updating.
		*
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	forceMaxUpdate		Navi normally only updates when the page has changed, to override this functionality
		*								set this parameter to 'True' to make Navi 'force update' using the value of the 
		*								parameter 'maxUpdatesPerSec'. This is useful as a work-around for rendering embedded 
		*								Flash applications. Note: if 'maxUpdatesPerSec' is 0, Navi will try to 'force update'
		*								every single chance it gets (not recommended). Set this to 'False' to make Navi update
		*								only when the page changes (auto-updating).
		*/
		void setForceMaxUpdate(const std::string &naviName, bool forceMaxUpdate = false);

		/**
		* Moves a movable non-material Navi by relative amounts.
		*
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	deltaX	The relative X amount to move the Navi by. Positive amounts move it right.
		*
		* @param	deltaY	The relative Y amount to move the Navi by. Positive amounts move it down.
		*/
		void moveNavi(const std::string &naviName, int deltaX, int deltaY);

		/** 
		* Sets the default position of a non-material Navi to a new position and subsequently moves
		* the Navi to this position. This will work on non-movable non-material Navis.
		*
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	naviPosition	The new NaviPosition to set the Navi to.
		*/
		void setNaviPosition(const std::string &naviName, const NaviPosition &naviPosition);

		/**
		* Resets the position of a movable Navi to the position it was created with.
		*
		* @param	naviName	The name of the Navi to do this to.
		*/
		void resetNaviPosition(const std::string &naviName);

		/**
		* Resets the positions of all movable Navis to the positions they were created with.
		*/
		void resetAllPositions();

		/**
		* Hides a Navi. For regular Navis, this also additionally hides its internal overlay.
		*
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	fade	Whether or not to fade the Navi down.
		*
		* @param	fadeDurationMS	If fading, the number of milliseconds to fade for.
		*/
		void hideNavi(const std::string &naviName, bool fade = false, unsigned short fadeDurationMS = 300);

		/**
		* Shows a Navi. For regular Navis, this also additionally shows its internal overlay.
		*
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	fade	Whether or not to fade the Navi up.
		*
		* @param	fadeDurationMS	If fading, the number of milliseconds to fade for.
		*/
		void showNavi(const std::string &naviName, bool fade = false, unsigned short fadeDurationMS = 300);

		/**
		* Checks whether or not a Navi is focused/selected.
		*
		* @return	True if a Navi is focused, False otherwise.
		*/
		bool isAnyNaviFocused();

		/**
		* Gets the name of the currently focused/selected Navi.
		*
		* @return	If a Navi is focused, returns the name of the Navi. Otherwise it returns an empty string ("").
		*/
		const std::string & getFocusedNaviName();

		/**
		* Gets the name of the Ogre Material that is used internally for a certain Navi.
		*
		* @param	naviName	The name of the Navi that you wish to find the Material name of.
		*
		* @return	If the Navi is found, returns the name of the internal Ogre Material, otherwise an empty string is returned.
		*/
		std::string getNaviMaterialName(const std::string &naviName);

		/**
		* Gets a pointer to the Ogre::OverlayContainer (Panel) that is used internally for a certain Navi.
		* Please remember that NaviMaterials have no Panel and thus, this function only works with full Navis.
		*
		* @param	naviName	The name of the Navi that you wish to retrieve the Panel of
		*
		* @return	If the Navi is found and it is NOT a NaviMaterial, returns a pointer to the Panel, otherwise 0 is returned.
		*/
		Ogre::PanelOverlayElement* getNaviInternalPanel(const std::string &naviName);

		/**
		* Gets the current visibility of the Navi.
		*
		* @param	naviName	The name of the Navi to get the visibility of.
		*
		* @return	Whether or not the Navi is visible. Additionally returns false if the Navi is not found.
		*/
		bool getNaviVisibility(const std::string &naviName);

		/**
		* Gets the derived UV's of the Navi's internal material/texture. On certain systems we must compensate for lack of
		* NPOT on the videocard by using the next-highest POT texture. Normal Navi's compensate their UV's accordingly
		* however NaviMaterials will need to adjust their own by use of this function.
		*/
		void getDerivedUV(const std::string &naviName, Ogre::Real& u1, Ogre::Real& v1, Ogre::Real& u2, Ogre::Real& v2);

		/**
		* Injects absolute mouse coordinates into NaviManager. Used to generally keep track of where the mouse 
		* is for things like moving Navis around, telling the internal pages of each Navi where the mouse is and
		* where the user has clicked, etc.
		*
		* @param	xPos	The absolute X-Value of the mouse.
		* @param	yPos	The absolute Y-Value of the mouse.
		*
		* @return	Returns True if the injected coordinate is over a Navi, False otherwise.
		*/
		bool injectMouseMove(int xPos, int yPos);

		/**
		* Injects absolute mouse coordinates (in the Navi's own local coordinate space) into a specific Navi/NaviMaterial.
		*
		* @param	naviName	The name of the Navi/NaviMaterial to inject this into.
		* @param	xPos	The absolute X-Value of the mouse, relative to the Navi/NaviMaterial's origin.
		* @param	yPos	The absolute Y-Value of the mouse, relative to the Navi/NaviMaterial's origin.
		*/
		void injectNaviMouseMove(const std::string &naviName, int xPos, int yPos);

		/**
		* Injects mouse wheel events into NaviManager. Used to scroll the focused Navi.
		*
		* @param	relScroll	The relative Scroll-Value of the mouse.
		*
		* @note
		*	To inject this using OIS: on a OIS::MouseListener::MouseMoved event, simply 
		*	inject "arg.state.Z.rel" of the "MouseEvent".
		*
		* @return	Returns True if the mouse wheel was scrolled while a Navi was focused, False otherwise.
		*/
		bool injectMouseWheel(int relScroll);

		/**
		* Injects mouse wheel events into a specific Navi/NaviMaterial.
		*
		* @param	naviName	The name of the Navi/NaviMaterial to inject this into.
		* @param	relScroll	The relative Scroll-Value of the mouse.
		*
		* @note
		*	To inject this using OIS: on a OIS::MouseListener::MouseMoved event, simply 
		*	inject "arg.state.Z.rel" of the "MouseEvent".
		*/
		void injectNaviMouseWheel(const std::string &naviName, int relScroll);

		/**
		* Injects mouse down events into NaviManager. Used to know when the user has pressed a mouse button
		* and which button they used.
		*
		* @param	buttonID	The ID of the button that was pressed. Left = 0, Right = 1, Middle = 2.
		*
		* @return	Returns True if the mouse went down over a Navi, False otherwise.
		*/
		bool injectMouseDown(int buttonID);

		/**
		* Injects mouse down events into a specific Navi/NaviMaterial.
		*
		* @param	naviName	The name of the Navi/NaviMaterial to inject this into.
		* @param	buttonID	The ID of the button that was pressed. Left = 0, Right = 1, Middle = 2.
		* @param	xPos	The absolute X-Value of the mouse, relative to the Navi/NaviMaterial's origin.
		* @param	yPos	The absolute Y-Value of the mouse, relative to the Navi/NaviMaterial's origin.
		*/
		void injectNaviMouseDown(const std::string &naviName, int buttonID, int xPos, int yPos);

		/**
		* Injects mouse up events into NaviManager. Used to know when the user has released a mouse button 
		* and which button they used.
		*
		* @param	buttonID	The ID of the button that was released. Left = 0, Right = 1, Middle = 2.
		*
		* @return	Returns True if the mouse went up while a Navi was focused, False otherwise.
		*/
		bool injectMouseUp(int buttonID);

		/**
		* Injects mouse up events into a specific Navi/NaviMaterial.
		*
		* @param	naviName	The name of the Navi/NaviMaterial to inject this into.
		* @param	buttonID	The ID of the button that was released. Left = 0, Right = 1, Middle = 2.
		* @param	xPos	The absolute X-Value of the mouse, relative to the Navi/NaviMaterial's origin.
		* @param	yPos	The absolute Y-Value of the mouse, relative to the Navi/NaviMaterial's origin.
		*/
		void injectNaviMouseUp(const std::string &naviName, int buttonID, int xPos, int yPos);

		/**
		* Subscribes a NaviEventListener to listen for events from a certain Navi.
		* 
		*
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	newListener		The address of the NaviEventListener. If you try to add the same 
		*							NaviEventListener twice, subsequent attempts will be ignored.
		*/
		void addNaviEventListener(const std::string &naviName, NaviEventListener* newListener);

		/**
		* Un-subscribes a NaviEventListener from a certain Navi.
		*
		* @param	naviName	The name of the Navi to do this to.
		*
		* @param	removeListener		The address of the NaviEventListener to remove.
		*/
		void removeNaviEventListener(const std::string &naviName, NaviEventListener* removeListener);

		/**
		* Binds the reception of a NaviData object from a certain Navi to a delegate function (callback).
		*
		* @param	naviName	The name of the Navi to receive the NaviData object from.
		*
		* @param	naviDataName	The name of the NaviData to bind the callback to.
		*
		* @param	callback	The NaviDelegate to bind to. NaviDelegates must return a 'void' and have one argument: 'const NaviData &naviData'
		*	\code
		*	// Member function instantiation:
		*	NaviDelegate(this, &MyClass::myMemberFunction)
		*
		*	// Static function instantiation:
		*	NaviDelegate(&myStaticFunction)
		*	\endcode
		*
		* @param	keys	An optional string vector containing the keys to ensure. See NaviData::ensure (second overload).
		* @note		It is highly advised to use the NaviUtilities::Strings typedef to invoke this parameter.
		*
		* @par
		*	An example:
		*	\code
		*	naviMgr.bind("chat", "messageSent", NaviDelegate(this, &NaviDemo::messageSent), Strings("nick")("message"));
		*	\endcode
		*/
//		void bind(const std::string &naviName, const std::string &naviDataName, const NaviDelegate &callback, const std::vector<std::string> &keys = std::vector<std::string>());

		/**
		* Un-binds the reception of a NaviData object from a certain Navi to a delegate function (callback)
		*
		* @param	naviName	The name of the Navi to unbind the NaviData from.
		*
		* @param	naviDataName	The name of the NaviData to unbind.
		*
		* @param	callback	The specific NaviDelegate to unbind. This is optional, if it is left blank, all bindings to
		*						'naviDataName' of 'naviName' will be released.
		*/
//		void unbind(const std::string &naviName, const std::string &naviDataName, const NaviDelegate &callback = NaviDelegate());

		/**
		* Focuses a Navi and pops it to the front of all other Navis.
		*
		* @param	naviName	The name of the Navi to focus.
		*/
		void focusNavi(const std::string &naviName);

		/**
		* De-Focuses any currently-focused Navis. This would be useful if you need to disable any auto-key-injection
		* (and subsequent display in a focused textbox of a focused Navi) done internally by Gecko.
		*/
		void deFocusAllNavis();
	};

}

#endif
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

#ifndef __Navi_H__
#define __Navi_H__
#if _MSC_VER > 1000
#pragma once
#endif

#include "NaviPlatform.h"
#include "NaviManager.h"
#include <llmozlib.h>

namespace NaviLibrary
{
	/**
	* This class is pure private, Navi objects are solely handled by NaviManager
	*/
	class _NaviExport Navi : public LLEmbeddedBrowserWindowObserver, public Ogre::WindowEventListener, public Ogre::ManualResourceLoader
	{
		friend class NaviManager;
		friend class NaviCompare;

		std::string naviName;
		unsigned short naviWidth;
		unsigned short naviHeight;
		unsigned int winWidth;
		unsigned int winHeight;
		Ogre::RenderWindow* renderWindow;
		bool isWinFocused;
		NaviPosition position;
		bool movable;
		int windowID;
		Ogre::Overlay* overlay;
		Ogre::PanelOverlayElement* panel;
		bool needsUpdate;
		unsigned int maxUpdatePS;
		bool forceMax;
		Ogre::Timer timer;
		unsigned long lastUpdateTime;
		float opacity;
		bool usingMask;
		bool ignoringTrans;
		float transparent;
		bool usingColorKeying;
		float keyFuzziness;
		unsigned char keyR, keyG, keyB;
		float keyFOpacity;
		unsigned char keyFillR, keyFillG, keyFillB;
		unsigned char* naviCache;
		bool isMaterialOnly;
		std::vector<NaviEventListener*> eventListeners;
//		std::multimap<std::string, NaviDelegate> delegateMap;
//		std::multimap<std::string, NaviDelegate>::iterator delegateIter;
//		std::pair<std::multimap<std::string, NaviDelegate>::iterator, std::multimap<std::string, NaviDelegate>::iterator> dmBounds;
		std::map<std::string, std::vector<std::string> > ensureKeysMap;
		std::map<std::string, std::vector<std::string> >::iterator ensureKeysMapIter;
		bool okayToDelete;
		bool isVisible;
		bool fadingOut;
		unsigned long fadingOutStart;
		unsigned long fadingOutEnd;
		bool fadingIn;
		unsigned long fadingInStart;
		unsigned long fadingInEnd;
		bool compensateNPOT;
		unsigned short texWidth;
		unsigned short texHeight;
		size_t texPixelSize;
		size_t texPitch;


		Navi(Ogre::RenderWindow* renderWin, std::string name, std::string homepage, const NaviPosition &naviPosition,
			unsigned short width, unsigned short height, bool isMovable, bool visible, unsigned int maxUpdatesPerSec, bool forceMaxUpdate, unsigned short zOrder, float _opacity);

		Navi(Ogre::RenderWindow* renderWin, std::string name, std::string homepage, unsigned short width, unsigned short height, bool visible,
			unsigned int maxUpdatesPerSec, bool forceMaxUpdate, float _opacity, Ogre::FilterOptions texFiltering);

		~Navi();

		void createOverlay(unsigned short zOrder);

		void createBrowser(Ogre::RenderWindow* renderWin, std::string homepage);

		void createMaterial(Ogre::FilterOptions texFiltering = Ogre::FO_NONE);

		void setMask(std::string maskFileName, std::string groupName);

		void update();

		void loadResource(Ogre::Resource* resource);

		void moveNavi(int deltaX, int deltaY);

		void navigateTo(std::string url);

		void navigateTo(std::string url, NaviData naviData);

		std::string evaluateJS(const std::string &script);

		void addEventListener(NaviEventListener* newListener);

		void removeEventListener(NaviEventListener* removeListener);

//		void bind(const std::string &naviDataName, const NaviDelegate &callback, const std::vector<std::string> &keys);

//		void unbind(const std::string &naviDataName, const NaviDelegate &callback = NaviDelegate());

		void setBackgroundColor(float red, float green, float blue);

		void setOpacity(float _opacity);

		void setIgnoreTransparentAreas(bool ignoreTrans, float defineThreshold);

		void setColorKey(const std::string &keyColor, float keyFillOpacity = 0.0, const std::string &keyFillColor = "#000000", float keyFuzzy = 0.0);

		void setDefaultPosition();

		void hide(bool fade, unsigned short fadeDurationMS);

		void show(bool fade, unsigned short fadeDurationMS);

		bool isPointOverMe(int x, int y);

		bool isPointWithin(int x, int y, int left, int right, int top, int bottom);

		bool isPointOpaqueEnough(int x, int y);

		int getRelativeX(int absX);

		int getRelativeY(int absY);

		void onPageChanged(const EventType& eventIn);
		void onNavigateBegin(const EventType& eventIn);
		void onNavigateComplete(const EventType& eventIn);
		void onUpdateProgress(const EventType& eventIn);
		void onStatusTextChange(const EventType& eventIn);
		void onLocationChange(const EventType& eventIn);
		void onClickLinkHref(const EventType& eventIn);

		void windowMoved(Ogre::RenderWindow* rw);
		void windowResized(Ogre::RenderWindow* rw);
		void windowClosed(Ogre::RenderWindow* rw);
		void windowFocusChange(Ogre::RenderWindow* rw);

	};

}

#endif
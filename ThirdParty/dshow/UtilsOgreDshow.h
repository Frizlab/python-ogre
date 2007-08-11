// Ogre Dshow: small wrapper for video reproduction in Ogre, using Direct Show 9.
/*
   Wrapper for video reproduction using Direct Show in the Ogre 3d engine.

   Coded by H. Hernán Moraldo from Moraldo Games
   www.hernan.moraldo.com.ar/pmenglish/field.php

   --------------------

   Copyright (c) 2007 Horacio Hernan Moraldo

   This software is provided 'as-is', without any express or
   implied warranty. In no event will the authors be held liable
   for any damages arising from the use of this software.

   Permission is granted to anyone to use this software for any
   purpose, including commercial applications, and to alter it and
   redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you
   must not claim that you wrote the original software. If you use
   this software in a product, an acknowledgment in the product
   documentation would be appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and
   must not be misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.

*/

#ifndef __FILE_UTILSOGREDSHOW_INCLUDED
#define __FILE_UTILSOGREDSHOW_INCLUDED


#define __FILE_UTILSOGREDSHOW_VERSION "1-30-2007b"

#include <Ogre.h>
#include <OgreVector2.h>

namespace OgreUtils
{
	struct DirectShowData;

	/// A class for playing movies in an ogre texture
	class DirectShowMovieTexture
	{
	public:
		// cons / decons
		/// Initializes the dshow object, and creates a texture with the given dimensions.
		/**
		If dontModifyDimensions is false, the system might modify the texture dimensions
		by setting them to the nearest power of two (useful for old computers).
		(Ie, 1024x512 if the original dimensions were 640x480).
		*/
		DirectShowMovieTexture(int width, int height, bool dontModifyDimensions=true);
		/// Destroys the dshow object
		virtual ~DirectShowMovieTexture();

		// basic movie methods
		/// Loads a given movie
		/**
		/param moviePath A string telling the full path of the file to be loaded.
		/param horizontalMirroring A bool telling whether the video should be rendered
		as if seen through a mirror, or not.
		*/
		void loadMovie(const Ogre::String& moviePath, bool horizontalMirroring=false);
		/// Obtains the dimensions of the current movie
		Ogre::Vector2 getMovieDimensions();
		/// Unloads the current movie
		void unloadMovie();

		// methods for movie control
		/// Pauses the current movie
		void pauseMovie();
		/// Starts playing the current movie
		void playMovie();
		/// Makes the current movie rewind
		void rewindMovie();
		/// Stops the current movie
		void stopMovie();
		/// Is the latest video put to play, now playing?
		/** (This is an old implementation of mine; I guess I should re-check this) */
		bool isPlayingMovie();

		// methods on movie texture
		/// Obtain the ogre texture where the movie is rendered
		Ogre::TexturePtr getMovieTexture();
		/// Render a movie frame in the ogre texture
		void updateMovieTexture();
	protected:
		/// Texture where to render the movie
		Ogre::TexturePtr mTexture;
		/// Real texture width
		Ogre::Real mTexWidth;
		/// Real texture height
		Ogre::Real mTexHeight;
		/// Direct Show specific data
		DirectShowData* dsdata;
		/// Do we do horizontal mirroring by software?
		bool mHorizontalMirroring;

		/// Clean the full texture (paint it all black)
		void cleanTextureContents();
	};
}


#endif // __FILE_UTILSOGREDSHOW_INCLUDED
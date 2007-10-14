//
//	NxOgre a wrapper for the PhysX (formerly Novodex) physics library and the Ogre 3D rendering engine.
//	Copyright (C) 2005 - 2007 Robin Southern and NxOgre.org http://www.nxogre.org
//
//	This library is free software; you can redistribute it and/or
//	modify it under the terms of the GNU Lesser General Public
//	License as published by the Free Software Foundation; either
//	version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//	Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public
//	License along with this library; if not, write to the Free Software
//	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifndef __NXOGRE_PARAMS_H__
#define __NXOGRE_PARAMS_H__

#include "NxOgrePrerequisites.h"

namespace NxOgre {

	
	class NxExport Params  {

		public:

			
			typedef std::pair<NxString,NxString> Parameter;
			typedef std::vector<Parameter> Parameters;
			
			void process(const NxString&);
			Parameters fromString(const NxString&);

			virtual void setToDefault() {}

		protected:

			virtual void parse(Parameters) {}

			bool Set(const NxString&, Parameter &, int&);
			bool Set(const NxString&, Parameter &, unsigned int&);
			bool Set(const NxString&, Parameter &, NxMaterialIndex&);
			bool Set(const NxString&, Parameter &, bool&);
			bool Set(const NxString&, Parameter &, NxReal&);
			bool Set(const NxString&, Parameter &, NxVec3&);
			bool Set(const NxString&, Parameter &, NxQuat&);
			bool Set(const NxString&, Parameter &, Ogre::Vector3&);
			bool Set(const NxString&, Parameter &, Ogre::Quaternion&);
			bool Set(const NxString&, Parameter &, NxString&);
			bool Set(const NxString&, Parameter &, NxHeightFieldAxis&);

			void toParameter(const NxString&, Parameter &d);
			void toLower(NxString&);

		private:

	};

	/*

		// 
		// Want Params in your application or engine? This is how you do it:-
		///////////////////////////////////////////////////////

		class NxExport myParams : Params {

		public:


			myParams() {setToDefault();}
			myParams(const char* p) {process(p);}
			myParams(NxString p) {process(p);}

			void setToDefault() {
				jump = true;
				jumpHeight = 2.35f;
			}


			void parse(Parameters P) {
				for (Parameters::iterator p = P.begin(); p != P.end();p++) {
					if (Set("jump", (*p), jump)	continue;
					if (Set("jump-height", (*p), jumpHeight) continue;
				}
			}

			bool jump;
			float jumpHeight;

		};

		///////////////////////////////////////////////////////


	*/
};

#endif

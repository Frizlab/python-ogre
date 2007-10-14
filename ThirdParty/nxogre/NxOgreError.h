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

#ifndef __NXOGRE_ERROR_H__
#define __NXOGRE_ERROR_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreContainer.h"

namespace NxOgre {

	///////////////////////////////////////////////////////////////////////

	struct ErrorReport {

		NxString Caller;
		NxString Message;

		enum ErrorType {
			ET_Conflic		= 0,
			ET_Warning		= 1,
			ET_FatalError	= 2,
			ET_Debug		= 3,
			ET_Leak			= 4
		};

		ErrorType		type;
		NxU32			frame;
		NxReal			second;

	};

	///////////////////////////////////////////////////////////////////////

	class NxExport Error : public NxUserOutputStream {

		public:

			Error(PhysXDriver*, bool shutdownOnErrors = false);
			~Error();

			unsigned int addReporter(ErrorReporter*,bool own);
			void report(const ErrorReport&);
			void setShutdownOnErrors(bool);

			ErrorReport	getNewReport();

			static Error* getInstance()							{
																	return mError;
																}

			// NxUserOutputStream bits.

			/// reportError
			void reportError (NxErrorCode code, const char *message, const char* file, int line);
			
			/// reportAssertViolation
			NxAssertResponse reportAssertViolation (const char *message, const char *file,int line);
			
			/// print
			void print (const char *message);
			
			/// getErrorCode
			NxString getErrorCode(NxErrorCode);

		protected:

			NxU32 nbWarnings;
			NxU32 nbErrors;
			bool ShutdownOnErrors;

			ErrorReporterList mReporters;

		private:

			PhysXDriver *mDriver;
			static Error* mError;
	};

	///////////////////////////////////////////////////////////////////////

	class NxExport ErrorReporter {

		public:

			ErrorReporter()										{}
			virtual ~ErrorReporter()							{}
			virtual void report(const ErrorReport&)				{}
	};

/*

	// Example
	class myNxOgreErrorSystem : public ErrorReporter {

		public:

			myNxOgreErrorSystem() {
				MyErrorSystem::getSingleton()->createErrorSubSection("Physics");
			}

			~myNxOgreErrorSystem () {
				MyErrorSystem::getSingleton()->destroyErrorSubSection("Physics");
			}

			void report(const ErrorReport&) {
				MyErrorSystem::getSingleton()->reportError("Physics", r.Caller, r.Message, r.Frame);
			}

	};

*/

};

#endif

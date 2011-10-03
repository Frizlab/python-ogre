/*
-----------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2006-2007 Henry van Merode

Usage of this program is free for non-commercial use and licensed under the
the terms of the GNU Lesser General Public License.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/

#ifndef __PU_HOOK_H__
#define __PU_HOOK_H__

#include "ParticleUniversePrerequisites.h"

namespace ParticleUniverse
{
	/** The Hook class is a template helper class that is used to call the hook functions of objects (Affectors, 
		Externs, ...) registered at the ParticleSystem. This prevents that the same code is duplicated 
		too much.
    */
	template <typename T>
	class _ParticleUniverseExport Hook
	{
		public:
			/** Templated version to prepare objects.
			*/
			static void _prepare(std::vector<T*>* list, ParticleTechnique* technique)
			{
				if (list->empty())
					return;
                typename T::iterator it;
                typename T::iterator itEnd = list->end();
				for (it = list->begin(); it != itEnd; ++it)
				{
					(*it)->_prepare(technique);
				}
			}

			/** Templated version to notify objects that the ParticleSystem starts
			*/
			static void _notifyStart(std::vector<T*>* list)
			{
				if (list->empty())
					return;
                typename T::iterator it;
                typename T::iterator itEnd = list->end();
				for (it = list->begin(); it != itEnd; ++it)
				{
					(*it)->_notifyStart();
				}
			}

			/** Templated version to notify objects that the ParticleSystem stops
			*/
			static void _notifyStop(std::vector<T*>* list)
			{
				if (list->empty())
					return;
                typename T::iterator it;
                typename T::iterator itEnd = list->end();
				for (it = list->begin(); it != itEnd; ++it)
				{
					(*it)->_notifyStop();
				}
			}

			/** Templated version of _preProcessParticles.
			@remarks
				_preProcessParticles allows actions before the individual particles are processed.
			*/
			static void _preProcessParticles(std::vector<T*>* list, ParticleTechnique* technique, Ogre::Real timeElapsed)
			{
				if (list->empty())
					return;
                typename T::iterator it;
                typename T::iterator itEnd = list->end();
				for (it = list->begin(); it != itEnd; ++it)
				{
					(*it)->_preProcessParticles(technique, timeElapsed);
				}
			}

			/** Templated version to process a particle.
			*/
			static void _processParticle(std::vector<T*>* list, 
				ParticleTechnique* particleTechnique, 
				Particle* particle, 
				Ogre::Real timeElapsed, 
				bool firstParticle)
			{
				if (list->empty())
					return;
                typename T::iterator it;
                typename T::iterator itEnd = list->end();
				for (it = list->begin(); it != itEnd; ++it)
				{
					(*it)->_processParticle(particleTechnique, particle, timeElapsed, firstParticle);
				}
			}

			/** Templated version of _postProcessParticles.
			@remarks
				_postProcessParticles allows actions after the individual particles are processed.
			*/
			static void _postProcessParticles(std::vector<T*>* list, ParticleTechnique* technique, Ogre::Real timeElapsed)
			{
				if (list->empty())
					return;
                typename T::iterator it;
                typename T::iterator itEnd = list->end();
				for (it = list->begin(); it != itEnd; ++it)
				{
					(*it)->_postProcessParticles(technique, timeElapsed);
				}
			}
	};

}
#endif

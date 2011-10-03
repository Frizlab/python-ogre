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

#ifndef __PU_DYNAMIC_ATTRIBUTE_FACTORY_H__
#define __PU_DYNAMIC_ATTRIBUTE_FACTORY_H__

#include "ParticleUniverseDynamicAttribute.h"

namespace ParticleUniverse
{
	/** This factory class is responsible for creating a DynamicAttribute object.
    */
	class _ParticleUniverseExport DynamicAttributeFactory
	{
	    public:

			DynamicAttributeFactory(void) {};
	        virtual ~DynamicAttributeFactory(void) {};

			/** 
	        */
			template <class T>
			T* createDynamicAttribute(void)
			{
				return new T();
			};


			/** 
	        */
			DynamicAttribute* cloneDynamicAttribute(DynamicAttribute* dynamicAttribute)
			{
				if (dynamicAttribute)
				{
					switch(dynamicAttribute->getType())
					{
						case DynamicAttribute::DAT_FIXED:
						{
							DynamicAttributeFixed* dynAttr = createDynamicAttribute<DynamicAttributeFixed>();
							dynamicAttribute->copyAttributesTo(dynAttr);
							return dynAttr;	
						}
						break;

						case DynamicAttribute::DAT_RANDOM:
						{
							DynamicAttributeRandom* dynAttr = createDynamicAttribute<DynamicAttributeRandom>();
							dynamicAttribute->copyAttributesTo(dynAttr);
							return dynAttr;
						}
						break;

						case DynamicAttribute::DAT_CURVED:
						{
							DynamicAttributeCurved* dynAttr = createDynamicAttribute<DynamicAttributeCurved>();
							dynamicAttribute->copyAttributesTo(dynAttr);
							return dynAttr;
						}
						break;
					}
				}

				return 0;
			}
			
			/** Delete a DynamicAttribute
	        */
			void destroyDynamicAttribute (DynamicAttribute* dynamicAttribute){delete dynamicAttribute;};
	};

}
#endif

/** \file    NxOgreContactCallback.h
 *  \brief   Header for the ContactCallback classes.
 *  \version 1.0-22T4
 *
 *  \licence NxOgre a wrapper for the PhysX physics library.
 *           Copyright (C) 2005-8 Robin Southern of NxOgre.org http://www.nxogre.org
 *           This library is free software; you can redistribute it and/or
 *           modify it under the terms of the GNU Lesser General Public
 *           License as published by the Free Software Foundation; either
 *           version 2.1 of the License, or (at your option) any later version.
 *           
 *           This library is distributed in the hope that it will be useful,
 *           but WITHOUT ANY WARRANTY; without even the implied warranty of
 *           MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *           Lesser General Public License for more details.
 *           
 *           You should have received a copy of the GNU Lesser General Public
 *           License along with this library; if not, write to the Free Software
 *           Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __NXOGRE_CONTACT_CALLBACK_CC_H__
#define __NXOGRE_CONTACT_CALLBACK_CC_H__

#include "NxOgrePrerequisites.h"

#ifdef WIN32
#  pragma pointers_to_members(full_generality, virtual_inheritance)
#endif

namespace NxOgre
{

  typedef void (NxOgre::ActorContactCallback::*ActorContactCallbackFunction)(NxOgre::Actor*, NxOgre::Actor*, NxContactPair&);

  /** \brief A very simple class that you inherit and bind your own functions to for the use in
             capturing contact events with Actors.
  */
  class NxPublicClass ActorContactCallback
  {
    public:

      /** \brief Types of callbacks in numerical form, used with mContactFunctions
      */
      enum CallbackType
      {
       _onBeginContact = 0,
       _onEndContact   = 1,
       _onContact      = 2
      };

     /** \brief Required Actor destructor
     */
     virtual ~ActorContactCallback();

     /** \brief ContactFunctions are used when a Contact is made, instead of inheritance or template
                programming. A simple function pointer is used to reduce the amount of conditional
                checking and function wrapping. It is up to you to implement the contact functions but
                they must have the same function signature.
         \example
          An example of the three contact functions, you may even merge them together as one.
         <code>
           void onBeginContact(Actor*, Actor*, NxContactPair&);
           void onEndContact(Actor*, Actor*, NxContactPair&);
           void onContact(Actor*, Actor*, NxContactPair&);
         </code>
         How to bind them
         <code>
           myClass::myClass()
           {
            mContactFunctions[_onBeginContact] = _ActorContactCallbackFunction(&myClass::onBeginContact);
            mContactFunctions[_onEndContact] = _ActorContactCallbackFunction(&myClass::onEndContact);
            mContactFunctions[_onContact] = _ActorContactCallbackFunction(&myClass::onContact);
           }
         </code>
     */
#if 1
  void (ActorContactCallback::*mContactFunctions[3])(NxOgre::Actor*, NxOgre::Actor*, NxContactPair&);
#else
    virtual void onBeginContact(Actor*, Actor*, NxContactPair&){}
    virtual void onEndContact(Actor*, Actor*, NxContactPair&){}
    virtual void onContact(Actor*, Actor*, NxContactPair&){}
#endif
  };

  /** \internal
  */
  class ActorContactCallbackAutoPtr
  {
   public:
     ActorContactCallback*   mCallback;
     GarbageCollectionPolicy mGarbageCollection;
  };

  /** \internal
  */
  class ActorContactSpecificCallbackAutoPtr
  {
   public:
    ActorContactCallback*   mCallback;           //< GarbageCollection thing.
    GarbageCollectionPolicy mGarbageCollection; //< GC
    Actor*                  mActorA;            //< Must be not NULL
    Actor*                  mActorB;            //< Can be null, then anything goes.
    NxU32                   mActorBWildcard;    //< Wildcard.
    bool                    mExclusive;         //< When matches, break out of the onContactNotify function.
  };


}; // End of namespace.

#endif 

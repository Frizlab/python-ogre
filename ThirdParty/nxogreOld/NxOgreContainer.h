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

#ifndef __NXOGRE_CONTAINER_H__
#define __NXOGRE_CONTAINER_H__

#include "NxOgrePrerequisites.h"
#include "OgreString.h"

namespace NxOgre {

	/** @brief A list of stored classes with an attached name with optional garbage collection.

		@example
		<code>

			// With: Object*														

			Container<NxString, Object*> Objects;										
			Objects myObjects;
			// ...
			for (Object* object = myObjects.begin();object = myObjects.next();) {
				// ...
			}

			// With: Object

			Container<NxString, Object> Objects
			Objects myObjects;
			// ...
			if (myObjects.count()) {
				for (Object object = myObjects._begin();myObjects._atEnd();object = myObjects._next()) {
					// ...
				}
			}
		</code>

	*/
	template <
				typename TI, // Identifier
				typename TT  // Type
			 >
	class Container {

		public:

			struct Containee {
				Containee() {}
				Containee(TT T, bool Owned) {t=T;owned=Owned;}
				TT t;
				bool owned;
			};
			////////////////////////////////////////////////////////////

			NX_INLINE Container<TI, TT>() {}
			NX_INLINE ~Container() {}

			////////////////////////////////////////////////////////////

			/** @brief Get an item from the container
				@param t	The identifier of the item
				@return The item or NULL if it does not exist
			*/
			NX_INLINE TT get(TI t) {
				if (has(t))
					return mItems[t].t;
				else
					return NULL;
			}

			////////////////////////////////////////////////////////////

			/** @brief Get the first item in the container, and reset the iterator.
				@return The first item.
			*/
			NX_INLINE TT getFirst() {
				Iterator it = mItems.begin();
				return it->second.t;
			}

			////////////////////////////////////////////////////////////

			/** @brief Returns if an item with an identifier exists.
				@param The identifier of the wanted item.
				@return If the item exists or not.
			*/
			NX_INLINE bool has(TI t) {
				Iterator i = mItems.find(t);
				if (i == mItems.end()) {
					return false;
				}
				return true;
			}

			////////////////////////////////////////////////////////////

			/** @brief Removes an item from the list. Does <strong>not</strong> delete the item.
				@param The identifier of the item to be deleted.
			*/
			NX_INLINE void remove(TI t) {
				mItems.erase(t);
			}

			////////////////////////////////////////////////////////////

			/** @brief Removes an item from the list. Does <strong>not</strong> delete the item.
				@param The identifier of the item to be deleted.
			*/
			NX_INLINE void insert(TI ts, TT t) {
				Iterator i = mItems.find(ts);
				if (i == mItems.end())
					mItems[ts] = Containee(t, false);
			}

			////////////////////////////////////////////////////////////

			
			/** @brief Resets the iterator, and returns the first item. For (Object*) use only.
			*/
			NX_INLINE TT begin() {
				mIterator = mItems.begin();
				if (mIterator != mItems.end())
					return mIterator->second.t;
				else
					return NULL;
			}

			////////////////////////////////////////////////////////////

			
			/** @brief From the current iterator position, increment and return the item. For (Object*) use only.
				@return The item or NULL when the iterator is at the past the end.
			*/
			NX_INLINE TT next() {
				if (mIterator == mItems.end())
					return NULL;

				return (mIterator++)->second.t;
			}

			////////////////////////////////////////////////////////////

			/** @brief Resets the iterator, and returns the first item. For (Object) use only.
				@return The first item.
			*/
			NX_INLINE TT _begin() {
				mIterator = mItems.begin();
				return (mIterator)->second.t;
			}

			////////////////////////////////////////////////////////////

			/** @brief From the current iterator position, increment and return the item. For (Object) use only.
				@return The item or NULL when the iterator is at the past the end.
			*/
			NX_INLINE TT _next() {
				return (mIterator++)->second.t;
			}

			////////////////////////////////////////////////////////////
			
			/** @brief If the iterator is at the end or not. For (Object) use only.
				@return The iterator is at the end or not.
			*/
			NX_INLINE bool _atEnd() {
				return (mIterator == mItems.end());
			}

			////////////////////////////////////////////////////////////

			
			/** @brief Returns the number of mItems in the container.
				@return The number of mItems.
			*/
			NX_INLINE NxU32 count() const {
				return mItems.size();
			}

			////////////////////////////////////////////////////////////

			/** @brief Clears everything in the list. Does <strong>not</strong> delete the mItems.
				@see Container<TI,TT>::destroyAllOwned
				@see Container<TI,TT>::destroyAndEraseAll
			*/
			NX_INLINE void empty() {
				mItems.clear();
			}

			////////////////////////////////////////////////////////////

			/** @brief Changes an identifier of an item with a new identifier
				@param a The item to be renamed.
				@param b The new identifier of the item.
			*/
			NX_INLINE void rename(TI a, TI b) {
				Iterator i = mItems.find(a);
				
				if (i == mItems.end())
					return;

				mItems[b] = Containee((*i).second.t, (*i).second.owned);
				mItems.erase(i);

			}

			////////////////////////////////////////////////////////////

			/** @brief "Locks" the item, turning it into an owned class for garbage collection.
			

				On the result of destroyAllOwned, these classes will be owned. All NxOgre
				classes created by it's factory method is owned by NxOgre, otherwise it
				is upto the developer to delete them at the correct time.

				@param t Identifier of the item
				@param b If it is owned or not.
				@see Container<TI,TT>::isLocked
			*/
			NX_INLINE void lock(TI t, bool b) {
				mItems[t].owned = b;
			}

			////////////////////////////////////////////////////////////

			/** @brief Returns if a class is owned/locked by the container.
				@param t Identifier of the item
				@see Container<TI,TT>::lock
			*/
			NX_INLINE bool isLocked(TI t) {
				 if (has(t))
					return mItems[t].owned;
				return false;
			}

			////////////////////////////////////////////////////////////

			/** @brief Prints out all of the mItems in the list to the developer console
				with identifier (if it is printable) and the class contents (a pointer is
				printed if it cannot be printed).

			*/
			NX_INLINE void dumpToConsole() {

				std::stringstream ss;
				ss << "Dump of Container:";
				ss << "-> Identifier type = " << typeid(TI).name() << std::endl << "-> Data Type = " << typeid(TT).name() << std::endl << std::endl;

				for(Iterator tt = mItems.begin();tt != mItems.end();tt++) {
					ss << "\t- Identifier => '" << (*tt).first << "', Value => '" << (*tt).second.t << "', Owned => " << (*tt).second.owned << std::endl;
				}

				ss << std::endl << "Done, " << mItems.size() << " item(s).";

				NxToConsole(ss.str());

			}

			////////////////////////////////////////////////////////////

			/** @brief Deletes all owned classes within the container.
				@note Does not remove the entry of the class from the container
					  as it is assumed once you've destroyAllOwned you are finished
					  with the container forever.

			*/
			NX_INLINE void destroyAllOwned() {

				if (mItems.size() == 0)
					return;

#if defined NX_DEBUG && (NX_TREAT_POSSIBLE_LEAKS_AS_LEAKS == 1)
				std::stringstream leaks;
				NxU32 leakCount = 0;
#endif

				for(Iterator tt = mItems.begin();tt != mItems.end();) {
					
					if ((*tt).second.owned) {
						delete (*tt++).second.t;
					}
					else {

#if defined NX_DEBUG && (NX_TREAT_POSSIBLE_LEAKS_AS_LEAKS == 1)
						leaks << "\t- " << (*tt).second.t << " => Indentifier '" << (*tt).first << "'." << std::endl;
						leakCount++;
#endif
						(*tt++);
					}
				}

#if defined NX_DEBUG && (NX_TREAT_POSSIBLE_LEAKS_AS_LEAKS == 1)
				if (leakCount>0) {
					std::stringstream formattedLeak;
					formattedLeak << "Possible leaks detected with non-owned class in a container" << std::endl << std::endl;
					formattedLeak << "-> Identifier type = " << typeid(TI).name() << std::endl 
								  << "-> Data Type = " << typeid(TT).name() << std::endl;
					formattedLeak << std::endl << leaks.str();
					formattedLeak << std::endl << leakCount << " leaks." << std::endl << std::endl << "To turn off this warning; disable \"NX_TREAT_POSSIBLE_LEAKS_AS_LEAKS\" in NxOgreStable.h" << std::endl;
					NxLeakDump(formattedLeak.str());
				}
#endif
				
			}

			////////////////////////////////////////////////////////////

			/** @brief Deletes every item regardless if it is owned or not.
			*/
			NX_INLINE void destroyAndEraseAll() {
			
				if (mItems.size() == 0)
					return;

				for(Iterator tt = mItems.begin();tt != mItems.end();) {
					delete (*tt++).second.t;
				}

				empty();

			}

			////////////////////////////////////////////////////////////

			/** @brief Copies the contents of this container to another.
				@example
				<code>
					Container<String, String> A;
					Container<String, String> B;
					...
					A.CopyTo(B);
				</code>
			*/
			NX_INLINE void CopyTo(Container<TI, TT>* c) {
				c->mItems = mItems;
				c->begin();
			}

			////////////////////////////////////////////////////////////

			/** @brief Copies the contents of this container to another.
				@example
				<code>
					Container<String, String> A;
					Container<String, String> B;
					...
					A.CopyTo(B);
				</code>
			*/
			NX_INLINE void CopyTo(Container<TI, TT>& c) {
				c.mItems = mItems;
				c.begin();
			}

			////////////////////////////////////////////////////////////

			
			/** @brief Operator access, equilvent to Container<TI,TT>::get
				@example
				<code>
					Container<String, String> A;
					...
					NxString foo = A["Foo"]
				</code>
			*/
			NX_INLINE TT operator[](TI ti) {
				return get(ti);
			}

			////////////////////////////////////////////////////////////

			typedef std::map<TI, Containee > TypeMap;
			typedef typename TypeMap::iterator Iterator;

			////////////////////////////////////////////////////////////

			TypeMap mItems;
			Iterator mIterator;

			////////////////////////////////////////////////////////////

	};

	template<typename TT /* Type */>
	class NxExport List {

		public:

			NxString get(NxString t) {
				if (has(t))
					return items[t];
				else
					return NULL;
			}


			////////////////////////////////////////////////////////////

			bool has(TT t) {
				Iterator i = items.find(t);
				if (i == items.end())
					return false;
				return true;
			}

			////////////////////////////////////////////////////////////

			void remove(TT t) {
				Iterator i = items.find(t);
				if (i != items.end())
					items.erase(i);
			}

			////////////////////////////////////////////////////////////

			void insert(TT t) {
				items.push_back(t);
			}

			////////////////////////////////////////////////////////////

			void begin() {
				mIterator = items.begin();
			}

			////////////////////////////////////////////////////////////

			TT next() {
				if (mIterator == items.end())
					return NULL;

				return mIterator->second++;
			}

			////////////////////////////////////////////////////////////

			unsigned int count() const {
				return items.size();
			}

			////////////////////////////////////////////////////////////

			void empty() {
				items.clear();
			}

			////////////////////////////////////////////////////////////

			void dumpToConsole() {

				std::stringstream ss;
				ss << "Dump of List:";
				ss << "-> Data Type = " << typeid(TT).name() << std::endl;

				NxU32 i=0;
				for(Iterator tt = items.begin();tt != items.end();tt++) {
					ss << "(" << i << ")  => '" << (*tt) << "'." << std::endl;
					i++;
				}

				ss << std::endl << "Done, " << items.size() << " item(s).";

				NxToConsole(ss.str());

			}

			////////////////////////////////////////////////////////////

			void destroyAndEraseAll() {
			
				if (items.size() == 0)
					return;

				for(Iterator tt = items.begin();tt != items.end();) {
					delete (*tt++);
				}

				empty();

			}

			////////////////////////////////////////////////////////////

			void EraseAll() {
				if (items.size() == 0)
					return;

				empty();
			}

			////////////////////////////////////////////////////////////

			typedef std::list<TT> Type;
			typedef typename Type::iterator Iterator;

			////////////////////////////////////////////////////////////

			Type items;
			Iterator mIterator;

			////////////////////////////////////////////////////////////

	};

	// Low-tech list of un-unique id string pairs.
	class StringPairList {

		public:

			typedef std::pair<NxString, NxString> StringPair;

			void parseCSString(const NxString& str) {

				static const std::string delims = " \t\r";
		
				std::vector<NxString> d = Ogre::StringUtil::split(str,",");
				NxString key, value;
				for(std::vector<NxString>::iterator it = d.begin();it != d.end();++it) {
		
					int dp = (*it).find_first_of(":");

					// Split
					key = (*it).substr(0,dp);
					value = (*it).substr(dp+1, (*it).length() - dp - 1);

					// Trim
					key.erase( key.find_last_not_of(delims)+1);
					key.erase(0,  key.find_first_not_of(delims));

					value.erase( value.find_last_not_of(delims)+1);
					value.erase(0,  value.find_first_not_of(delims));

					// Lower case first.
					std::transform(key.begin(),key.end(),key.begin(),tolower);

					insert(key, value);

				}

			}

			////////////////////////////////////////////////////////////
			
			NxString get(NxString key) {
				for(Type::iterator tt = items.begin();tt != items.end();tt++) {
					if ((*tt).first == key)
						return (*tt).second;
				}
				return "";
			}

			////////////////////////////////////////////////////////////

			bool end() {
				return (mIterator == items.end());
			}

			////////////////////////////////////////////////////////////

			void insert(NxString ts, NxString t) {
				items.push_back(StringPair(ts, t));
			}

			////////////////////////////////////////////////////////////

			StringPair begin() {
				mIterator = items.begin();

				return (*mIterator);
			}

			////////////////////////////////////////////////////////////

			bool hasKey(NxString key) {
				for(Type::iterator tt = items.begin();tt != items.end();tt++) {
					if ((*tt).first == key)
						return true;
				}
				return false;
			}

			////////////////////////////////////////////////////////////

			NxString getFirstOf(NxString key) {
				for(Type::iterator tt = items.begin();tt != items.end();tt++) {
					if ((*tt).first == key)
						return (*tt).second;
				}
				return "";
			}

			////////////////////////////////////////////////////////////

			bool hasNext() {
				return (mIterator != items.end());
			}

			////////////////////////////////////////////////////////////

			StringPair next() {
				return (*mIterator++);
			}

			////////////////////////////////////////////////////////////

			NxU32 count() const {
				return items.size();
			}

			////////////////////////////////////////////////////////////

			void empty() {
				items.clear();
			}

			////////////////////////////////////////////////////////////

			void dumpToConsole() {

				std::stringstream ss;
				ss << "Dump of Container:" << std::endl << std::endl;
				
				for(Type::iterator tt = items.begin();tt != items.end();tt++) {
					ss << "\t- Identifier => '" << (*tt).first << "', Value => '" << (*tt).second << "'" << std::endl;
				}

				ss << std::endl << "Done, " << items.size() << " item(s).";

				NxToConsole(ss.str());

			}

			////////////////////////////////////////////////////////////

			typedef std::list< StringPair > Type;
			typedef Type::iterator Iterator;

			////////////////////////////////////////////////////////////

			Type items;
			Type::iterator mIterator;

			////////////////////////////////////////////////////////////

	};

};

#endif

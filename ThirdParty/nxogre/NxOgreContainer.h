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

namespace NxOgre {

	/** Container




		How to use the iterator interface:

			With: Object*

			Container<NxString, Object*> Objects;
			Objects myObjects;
			...
			for (Object* object = myObjects.begin();object = myObjects.next();) {
				..
			}

			With: Object

			Container<NxString, Object> Objects
			Objects myObjects;
			...
			if (myObjects.count()) {
				for (Object object = myObjects._begin();myObjects._atEnd();object = myObjects._next()) {

				}
			}


	*/
	template <
				typename TI, // Identifier
				typename TT  // Type
			 >
	class NxExport Container {

		public:

			struct Containee {
				Containee() {}
				Containee(TT T, bool Owned) {t=T;owned=Owned;}
				TT t;
				bool owned;
			};

			////////////////////////////////////////////////////////////

			TT get(TI t) {
				if (has(t))
					return items[t].t;
				else
					return NULL;
			}

			////////////////////////////////////////////////////////////

			TT getFirst() {
				Iterator it = items.begin();
				return it->second.t;
			}

			////////////////////////////////////////////////////////////

			bool has(TI t) {
				Iterator i = items.find(t);
				if (i == items.end()) {
					return false;
				}
				return true;
			}

			////////////////////////////////////////////////////////////

			void remove(TI t) {
				//Iterator i = items.find(t); 
				//if (i != items.end()) {
				//	items.erase(i);
				//}

				items.erase(t);

			}

			////////////////////////////////////////////////////////////

			void insert(TI ts, TT t) {
				Iterator i = items.find(ts);
				if (i == items.end()) {
					items[ts] = Containee(t, false);
				}
			}

			////////////////////////////////////////////////////////////

			TT begin() {
				mIterator = items.begin();
				if (mIterator != items.end())
					return mIterator->second.t;
				else
					return NULL;
			}

			////////////////////////////////////////////////////////////

			TT next() {
				if (mIterator == items.end())
					return NULL;

				return (mIterator++)->second.t;
			}

			////////////////////////////////////////////////////////////

			TT _begin() {
				mIterator = items.begin();
				return (mIterator)->second.t;
			}

			////////////////////////////////////////////////////////////

			TT _next() {
				return (mIterator++)->second.t;
			}

			////////////////////////////////////////////////////////////

			bool _atEnd() {
				return (mIterator == items.end());
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

			void rename(TI a, TI b) {
				Iterator i = items.find(a);
				
				if (i == items.end())
					return;

				items[b] = Containee((*i).second.t, (*i).second.owned);
				items.erase(i);

			}

			////////////////////////////////////////////////////////////

			void lock(TI t, bool b) {
				items[t].owned = b;
			}

			////////////////////////////////////////////////////////////

			bool isLocked(TI t) {
				 if (has(t))
					return items[t].owned;
				return false;
			}

			////////////////////////////////////////////////////////////

			void dumpToConsole() {

				std::stringstream ss;
				ss << "Dump of Container:";
				ss << "-> Identifier type = " << typeid(TI).name() << std::endl << "-> Data Type = " << typeid(TT).name() << std::endl << std::endl;

				for(Iterator tt = items.begin();tt != items.end();tt++) {
					ss << "\t- Identifier => '" << (*tt).first << "', Value => '" << (*tt).second.t << "', Owned => " << (*tt).second.owned << std::endl;
				}

				ss << std::endl << "Done, " << items.size() << " item(s).";

				NxToConsole(ss.str());

			}

			////////////////////////////////////////////////////////////

			void destroyAllOwned() {

				if (items.size() == 0)
					return;

#if defined NX_DEBUG && defined NX_TREAT_POSSIBLE_LEAKS_AS_LEAKS
				std::stringstream leaks;
				NxU32 leakCount = 0;
#endif

				for(Iterator tt = items.begin();tt != items.end();) {
					
					if ((*tt).second.owned) {
						delete (*tt++).second.t;
					}
					else {

#if defined NX_DEBUG && defined NX_TREAT_POSSIBLE_LEAKS_AS_LEAKS
						leaks << "\t- " << (*tt).second.t << " => Indentifier '" << (*tt).first << "'." << std::endl;
						leakCount++;
#endif
						(*tt++);
					}
				}

#if defined NX_DEBUG && defined NX_TREAT_POSSIBLE_LEAKS_AS_LEAKS
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

			void destroyAndEraseAll() {
			
				if (items.size() == 0)
					return;

				for(Iterator tt = items.begin();tt != items.end();) {
					delete (*tt++).second.t;
				}

				empty();

			}

			////////////////////////////////////////////////////////////

			void CopyTo(Container<TI, TT>* c) {
				c->items = items;
				c->begin();
			}

			////////////////////////////////////////////////////////////

			void CopyTo(Container<TI, TT>& c) {
				c.items = items;
				c.begin();
			}

			////////////////////////////////////////////////////////////

			TT operator[](TI ti) {
				return get(ti);
			}

			////////////////////////////////////////////////////////////

			typedef std::map<TI, Containee > Type;
			typedef typename Type::iterator Iterator;

			////////////////////////////////////////////////////////////

			Type items;
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
				items.empty();
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

/*

Betajaen's SharedList

Copyright (c) 2007 Robin Southern, http://www.nxogre.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifndef BETAJAENS_SL_H
#define BETAJAENS_SL_H

#include <cstdlib>
#include "NxOgrePlatform.h"
#include "NxOgreAllocator.h"

// Configuration

/** \brief Namespace to place the Containers in. Comment out if you want it in the global namespace.
*/
#define BETAJAEN_SL_NAMESPACE Betajaen


/** \brief Export Class Macro, replace value with your export macro
*/
#define BETAJAEN_SL_CLASS // __declspec(dllexport)
#define BETAJAEN_SL_CLASS_FUNCTION
#define BETAJAEN_SL_TEMPLATE_CLASS 
#define BETAJAEN_SL_TEMPLATE_CLASS_FUNCTION // __forceinline
#define BETAJAEN_SL_CLASS_EXPORT // __declspec(dllexport)
#define BETAJAEN_SL_TEMPLATE_CLASS_EXPORT // __forceinline
// __declspec(dllexport)

/** \brief Delete macro. Used if you have your own leak checker, etc.
*/
// #define BETAJAEN_SL_DELETE(ptr) \
// 	{::NxOgre::Allocator::getPtr()->watch_delete(static_cast<void*>(ptr));}; \
// 	delete ptr;
// 
// /** \brief Called just before when a class is destroyed using it's destructor.
// */
// #define BETAJAEN_SL_DESTRUCTOR(ptr) \
// 	::NxOgre::Allocator::getPtr()->watch_delete(begin);

//////////////////////////////////////////////////////////////////////////

#ifdef BETAJAEN_SL_NAMESPACE
#   define BETAJAEN_SL_NAMESPACE_START namespace BETAJAEN_SL_NAMESPACE {
#   define BETAJAEN_SL_NAMESPACE_END }
#else
#   define BETAJAEN_SL_NAMESPACE_START /**/
#   define BETAJAEN_SL_NAMESPACE_END /**/
#endif

BETAJAEN_SL_NAMESPACE_START

#if (_MSC_VER>=1000)
#   define BETAJAEN_SL_INLINE __forceinline
#else
#   define BETAJAEN_SL_INLINE inline
#endif

#ifndef BETAJAEN_SL_CLASS
#   define BETAJAEN_SL_CLASS
#endif

#ifndef BETAJAEN_SL_TEMPLATE_CLASS
#   define BETAJAEN_SL_TEMPLATE_CLASS
#endif

#ifndef BETAJAEN_SL_TEMPLATE_CLASS_FUNCTION
#   define BETAJAEN_SL_TEMPLATE_CLASS_FUNCTION
#endif

#ifndef BETAJAEN_SL_DELETE
#   define BETAJAEN_SL_DELETE(ptr) delete ptr;
#endif

#ifndef BETAJAEN_SL_DESTRUCTOR
#   define BETAJAEN_SL_DESTRUCTOR(ptr)
#endif

BETAJAEN_SL_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

BETAJAEN_SL_NAMESPACE_START

class SharedAllocator;
template <class TypePtr, class AllocatorType = SharedAllocator> class Flat;
template <class BaseType, class AllocatorType = SharedAllocator> class Array;
template <class BaseType, class AllocatorType = SharedAllocator> class SharedList;

BETAJAEN_SL_NAMESPACE_END


//////////////////////////////////////////////////////////////////////////

BETAJAEN_SL_NAMESPACE_START

class BETAJAEN_SL_CLASS_EXPORT SharedAllocator {

public:

/////////////////////////////////////////////////////////////

void* Allocate(size_t size)
{
	return malloc(size);
}

/////////////////////////////////////////////////////////////

void Deallocate(void* mem)
{
	if (mem)
		free(mem);
}

/////////////////////////////////////////////////////////////

void* Reallocate(void* mem, size_t size)
{
	return realloc(mem, size);
}

/////////////////////////////////////////////////////////////

void Copy(void* dest, void* src, size_t length)
{
	char* d = (char*) dest;
	const char* s = (const char*) src;

	while(length-- != 0)
		*d++ = *s++;
}

}; // End of the SharedAllocator class.

BETAJAEN_SL_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

BETAJAEN_SL_NAMESPACE_START

template
		<
			class Type,
			class AllocatorType
		>
class BETAJAEN_SL_CLASS_EXPORT Flat
{

public:

typedef Type * Iterator;
typedef AllocatorType* AllocatorPtr;

/////////////////////////////////////////////////////////////

BETAJAEN_SL_INLINE Flat(AllocatorPtr allocator, bool deleteAllocator)
	: First(NULL), Last(NULL), End(NULL), Allocator(allocator), AutoDeleteAllocator(deleteAllocator)
{
}

/////////////////////////////////////////////////////////////

BETAJAEN_SL_INLINE ~Flat()
{
	_deallocate(First);
	if (AutoDeleteAllocator)
		delete Allocator;
}

/////////////////////////////////////////////////////////////

BETAJAEN_SL_INLINE unsigned int _size() const
{
	return (unsigned int) (Last - First);
}

/////////////////////////////////////////////////////////////

BETAJAEN_SL_INLINE unsigned int _capacity() const
{
	return (unsigned int) (First == 0 ? 0 : End - First);
}

/////////////////////////////////////////////////////////////

BETAJAEN_SL_INLINE Type* _allocate(unsigned int size)
{
	return (Type*) Allocator->Allocate(size * sizeof(Type));
}

/////////////////////////////////////////////////////////////

BETAJAEN_SL_INLINE void _deallocate(Type* p)
{
	if (p)
		Allocator->Deallocate(p);
}

/////////////////////////////////////////////////////////////

BETAJAEN_SL_INLINE Iterator _reallocate(Iterator p, unsigned int size)
{
	return (Iterator) Allocator->Reallocate(p, size  * sizeof(Type));
}

/////////////////////////////////////////////////////////////

BETAJAEN_SL_INLINE void _clip(unsigned int size)
{
	
	if (size < _size())
		return;

	Iterator new_first = _allocate(size);
	_copy(First, Last, new_first);
	_destroy(First, Last);
	_deallocate(First);
	End = new_first + size;
	Last = new_first + _size();
	First = new_first;

}

/////////////////////////////////////////////////////////////

BETAJAEN_SL_INLINE void _reserve(unsigned int new_size)
{
	if (_capacity() >= new_size)
		return;

	Iterator new_first = _allocate(new_size);
	_copy(First, Last, new_first);
	_destroy(First, Last);
	_deallocate(First);
	End = new_first + new_size;
	Last = new_first + _size();
	First = new_first;

}

/////////////////////////////////////////////////////////////

BETAJAEN_SL_INLINE Iterator _copy(Iterator begin, Iterator end, Iterator dest)
{
	for (;begin != end; ++dest, ++begin)
		*dest = *begin;
	return dest;
}

/////////////////////////////////////////////////////////////

BETAJAEN_SL_INLINE void _destroy(Iterator begin, Iterator end)
{
	for (; begin != end; ++begin) {
		BETAJAEN_SL_DESTRUCTOR(begin)
		begin->~Type();
	}
}

/////////////////////////////////////////////////////////////

public: BETAJAEN_SL_INLINE void _delete_range(Iterator begin, Iterator end)
{
	for (; begin != end; ++begin) {
		BETAJAEN_SL_DELETE(*begin);
	}
}

protected: AllocatorPtr Allocator;
protected: bool AutoDeleteAllocator;
public:  Iterator First, Last, End;

}; // End of Flat<Type, AllocatorType> class.

BETAJAEN_SL_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

BETAJAEN_SL_NAMESPACE_START

template
		<
			class Type,					/* Class to Store; A, myClassName, etc. */
			class AllocatorType			/* Allocator to use, or leave blank for SharedAllocator */
		>
class BETAJAEN_SL_CLASS_EXPORT Array {

public:

typedef Type* TypePtr;
typedef const Type* ConstTypePtr;
typedef Array<Type, AllocatorType> ArrayType;
typedef Flat<Type, AllocatorType> FlatType;
typedef FlatType* FlatPtr;

BETAJAEN_SL_INLINE Array(AllocatorType* _allocator = NULL, bool _auto_delete_allocator = true)
{
	if (_allocator == NULL)
		_allocator = new AllocatorType();

	_Flat = new FlatType(_allocator, _auto_delete_allocator);

}

BETAJAEN_SL_INLINE ~Array()
{
	delete _Flat;
}

BETAJAEN_SL_INLINE TypePtr getPtr()
{
	return _Flat->First;
}

BETAJAEN_SL_INLINE ConstTypePtr getConstPtr() const 
{
	return _Flat->First;
}

BETAJAEN_SL_INLINE unsigned int Size()
{
	return _Flat->_size();
}

BETAJAEN_SL_INLINE void Append(Type v) {

	if (_Flat->End <= _Flat->Last)
		_Flat->_reserve( (1 + _Flat->_size()) * 2);

	*_Flat->Last = v;
	_Flat->Last++;

}

BETAJAEN_SL_INLINE void Append(Type v, size_t length)
{
	if (_Flat->End <= _Flat->Last + length)
		_Flat->_reserve( length + (_Flat->_size() * 2));

	memcpy(*_Flat->Last, v, length);
	_Flat->Last += length;

}

BETAJAEN_SL_INLINE void Clip() {
	_Flat->_clip(0);
}

private: FlatPtr _Flat;

};

BETAJAEN_SL_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

BETAJAEN_SL_NAMESPACE_START

/** \brief A list of pointers with garbage collections and the ability to
	share the same list with similar lists. Like STL's Vector.

	\example
		<code>
			class A;

			SharedList<A> aList;
			a.Insert(new A());
			a.Remove(a.Last);
		</code>

*/
template
		<
			class Type,					/* Class to Store; A, myClassName, etc. */
			class AllocatorType			/* Allocator to use, or leave blank for SharedAllocator */
		>
class BETAJAEN_SL_CLASS_EXPORT SharedList {

public:
	
typedef Type* TypePtr;
typedef TypePtr* Iterator;
typedef SharedList<Type, AllocatorType> SharedListType;
typedef Flat<TypePtr, AllocatorType> FlatType;
typedef FlatType* FlatPtr;

/////////////////////////////////////////////////////////////

enum AbsolutePosition
{
	First,
	Last
};


/** \brief Iterator interface
*/
class BETAJAEN_SL_CLASS_EXPORT SharedListIterator {

/** \brief Normal constructor
*/
public: BETAJAEN_SL_INLINE SharedListIterator(FlatPtr flat)
: _Flat(flat), _Iterator(flat->First)
{
}

/** \brief Copy constructor
*/
public: BETAJAEN_SL_INLINE SharedListIterator(const SharedListIterator& other)
: _Flat(other._Flat), _Iterator(other._Flat->First)
{
}

/** \brief Destructor
*/
public: BETAJAEN_SL_INLINE ~SharedListIterator()
{
}

/** \brief First type in the SharedList without modifying the iterator
*/
public: BETAJAEN_SL_INLINE TypePtr FirstType() {
	return _Flat->First;
}

/** \brief Last type in the SharedList without modifying the iterator
*/
public: BETAJAEN_SL_INLINE TypePtr LastType() {
	return _Flat->Last;
}

/** \brief Places the Iterator at the beginning of the List
*/
public: BETAJAEN_SL_INLINE TypePtr Begin()
{

	if (_Flat->_size() == 0)
		return NULL;

	_Iterator = _Flat->First;
	return *_Iterator;

}


/** \brief Advances the Iterator forward by one, and returns the value of the list or NULL
	\returns The next item in the list, or NULL if it has reached the end of it.
*/
public: BETAJAEN_SL_INLINE TypePtr Next()
{

	if (_Flat->_size() == 0)
		return NULL;
	else if (_Iterator == _Flat->Last)
		return NULL;
	else
		return (*_Iterator++);

}


/** \brief Places the iterator at the end of the List
*/
public: BETAJAEN_SL_INLINE TypePtr End()
{

	if (_Flat->_size() == 0)
		return NULL;

	_Iterator = _Flat->Last - 1;
	return *_Iterator;

}


/** \brief Advances the Iterator backwards by one, and returns the value of the list or NULL
	\returns The previous item in the list, or NULL if it has reached the beginning of it.
*/
public: BETAJAEN_SL_INLINE TypePtr Previous()
{

	if (_Flat->_size() == 0)
		return NULL;
	else if (_Iterator == _Flat->First - 1)
		return NULL;
	else
		return (*_Iterator--);

}

/** \brief Returns the numbers of Type associcated to the flat with this iterator
*/
public: BETAJAEN_SL_INLINE unsigned int Size()
{
  return _Flat->_size();
}

protected: FlatPtr  _Flat;
protected: Iterator _Iterator;

};

/////////////////////////////////////////////////////////////////

typedef SharedListIterator SharedListIteratorType;

/** \brief Constructor with optional custom allocator
*/
BETAJAEN_SL_INLINE SharedList(AllocatorType* _allocator = NULL, bool _auto_delete_allocator = true)
{
	if (_allocator == NULL)
		_allocator = new AllocatorType();

	_Flat = new FlatType(_allocator, _auto_delete_allocator);
	_Usage = new unsigned int(1);

}

/** \brief Copy Constructor between SharedList's. The Flat will not be copied but referenced.
	Any action on this list will effect all lists using the same Flat. Clean up will only
	happen on the destruction of the last surviving list.

	\example
			<code>
				typedef SharedList<A> AList;
				AList a;
				AList b(a);
			</code>
*/
BETAJAEN_SL_INLINE SharedList(const SharedListType& other)
{

	_Flat = other._Flat;
	_Usage = other._Usage;
	++(*_Usage);

}


/** \brief	Copy Operator. Functions the same as Copy Constructor but deletes the set that this
			may already have.

	\example
			<code>
				typedef SharedList<A> AList;
				AList a;
				AList b;
				b = a;
			</code>
*/
BETAJAEN_SL_INLINE SharedListType& operator=(const SharedListType& other)
{

	if (_Flat)
	{
		_Flat->_delete_range(_Flat->First, _Flat->Last);
		delete _Flat;
	}

	if (_Usage)
		delete _Usage;

	_Flat = other._Flat;
	_Usage = other._Usage;
	++(*_Usage);

	return *this;

}


/** \brief Destructor. If this is the last of all SharedLists sharing the same set,
	the set will be destroyed otherwise the Flat will be ignored and this SharedList
	dies gracefully.
*/
BETAJAEN_SL_INLINE ~SharedList()
{
	
	if (--(*_Usage) == 0)
	{
		if (_Flat)
		{
			_Flat->_delete_range(_Flat->First, _Flat->Last);
			delete _Flat;
			delete _Usage;
		}
	}
	
}


/** \brief Gets the pointer to the nth Type in the list. Alternate interface to operator[]. 
*/
BETAJAEN_SL_INLINE TypePtr Get(size_t pos)
{
	if (pos > Size())
		return NULL;

	return *(_Flat->First + pos);
}


/** \brief Gets the pointer to the nth TypePtr in the list.
*/
BETAJAEN_SL_INLINE TypePtr operator[](size_t pos)
{
	if (pos > Size())
		return NULL;

	return *(_Flat->First + pos);
}


/** \brief Where is something in the list.
*/
BETAJAEN_SL_INLINE unsigned int WhereIs(TypePtr ptr)
{

	if (_Flat->_size() == 0)
		return 0;

	unsigned int i=0;
	for(Iterator begin = _Flat->First; begin != _Flat->Last; ++begin, ++i)
	{
		if (ptr == (*begin))
			return i;
	}
	
	return 0;
}

BETAJAEN_SL_INLINE bool Has(TypePtr ptr)
{
	
	if (_Flat->_size() == 0)
		return false;

		for(Iterator begin = _Flat->First; begin != _Flat->Last; ++begin)
		{
			if (ptr == (*begin))
				return true;
		}

	return false;

}

/** \brief The size of the SharedList.
*/
BETAJAEN_SL_INLINE unsigned Size() const
{
	return _Flat->_size();
}


/** \brief Insert something into the list
*/
BETAJAEN_SL_INLINE void Insert(const TypePtr& val)
{

	if (_Flat->End <= _Flat->Last)
		_Flat->_reserve( (1 + Size()) * 2);

	*_Flat->Last = val;
	_Flat->Last++;

}


/** \brief Resize the capacity of the list to size + n.
*/
BETAJAEN_SL_INLINE void Clean(unsigned int n = 4)
{
	_Flat->_clip(_Flat->_size() + n);
}


/** \brief Destroy something nth position into the list.
*/
BETAJAEN_SL_INLINE void Destroy(unsigned int n)
{
	if (n == 0)
		return Destroy(First);
	
	if (n == Size())
		return Destroy(Last);

	delete *(_Flat->First + n);

	_Flat->_destroy(_Flat->First + n, _Flat->First + n + 1);
	_Flat->_copy(_Flat->First + n + 1, _Flat->Last, _Flat->First + n);
	_Flat->Last--;

}

// \brief Destroys everything
//
BETAJAEN_SL_INLINE void DestroyAll()
{
	if (Size() == 0)
		return;

	_Flat->_delete_range(_Flat->First, _Flat->Last);
	// Could be iffy.
	_Flat->Last = _Flat->First;

}

/** \brief Destroys something in the list based on the pointer in the list.
*/
BETAJAEN_SL_INLINE void Destroy(TypePtr val) {
	unsigned int i = WhereIs(val);
	Destroy(i);
}

/** \brief Deletes something from the list based on AbsolutePosition
*/
BETAJAEN_SL_INLINE void Destroy(AbsolutePosition position)
{

	switch(position) {

		case First:
			{
				delete *_Flat->First;
				_Flat->_copy(_Flat->First + 1, _Flat->Last + 1, _Flat->First);
				_Flat->Last--;
			}
		break;

		case Last:
			{
				_Flat->Last--;
				delete *_Flat->Last;
			}
		break;

	}

}


/** \brief Removes something from the list based on numerical position.
*/
BETAJAEN_SL_INLINE TypePtr Remove(unsigned int n)
{

	if (n == 0)
		return Remove(First);
	
	if (n == Size())
		return Remove(Last);

	TypePtr ptr = Get(n);
	_Flat->_destroy(_Flat->First + n, _Flat->First + n + 1);
	_Flat->_copy(_Flat->First + n + 1, _Flat->Last, _Flat->First + n);
	_Flat->Last--;

	return ptr;
}


/** \brief Removes something from the list based on AbsolutePosition
*/
BETAJAEN_SL_INLINE TypePtr Remove(AbsolutePosition position)
{

	if (_Flat->_size() == 0)
		return NULL;

	switch(position) {

		case First:
			{
				TypePtr t = *(_Flat->First);
				_Flat->_copy(_Flat->First + 1, _Flat->Last + 1, _Flat->First);
				_Flat->Last--;
				return t;
			}

		break;

		case Last:
			{
				_Flat->Last--;
				return *(_Flat->Last);
			}
		break;
	}

	return NULL;

}

// \brief Destroys everything
//
BETAJAEN_SL_INLINE void RemoveAll()
{
	if (Size() == 0)
		return;

	_Flat->_deallocate(_Flat->First);

	// Could be iffy.
	_Flat->Last = _Flat->First = _Flat->End = NULL;

}


/** \brief Places the Iterator at the beginning of the List
*/
BETAJAEN_SL_INLINE TypePtr Begin()
{

	if (_Flat->_size() == 0)
		return NULL;

	_Iterator = _Flat->First;
	return *_Iterator;

}


/** \brief Advances the Iterator forward by one, and returns the value of the list or NULL
	\returns The next item in the list, or NULL if it has reached the end of it.
*/
BETAJAEN_SL_INLINE TypePtr Next()
{

	if (_Flat->_size() == 0)
		return NULL;
	else if (_Iterator == _Flat->Last)
		return NULL;
	else
		return (*_Iterator++);

}


/** \brief Places the iterator at the end of the List
*/
BETAJAEN_SL_INLINE TypePtr End()
{

	if (_Flat->_size() == 0)
		return NULL;

	_Iterator = _Flat->Last - 1;
	return *_Iterator;

}


/** \brief Advances the Iterator backwards by one, and returns the value of the list or NULL
	\returns The previous item in the list, or NULL if it has reached the beginning of it.
*/
BETAJAEN_SL_INLINE TypePtr Previous()
{

	if (_Flat->_size() == 0)
		return NULL;
	else if (_Iterator == _Flat->First - 1)
		return NULL;
	else
		return (*_Iterator--);

}

/** \brief For each item in the list call it's own method.
*/
BETAJAEN_SL_INLINE void Each(void (Type::*MethodPtr)()) {
	
	if (_Flat->_size() == 0)
		return;

	for(Iterator begin = _Flat->First; begin != _Flat->Last; ++begin) {
		(*begin->*MethodPtr)();
	}

}


/** \brief For each item in the list call it's own method, with one argument.
*/
template <typename _A>
BETAJAEN_SL_INLINE void Each(void (Type::*MethodPtr)(_A), _A _1) {

	if (_Flat->_size() == 0)
		return;

	for(Iterator begin = _Flat->First; begin != _Flat->Last; ++begin) {
		(*begin->*MethodPtr)(_1);
	}

}


/** \brief For each item in the list call it's own method, with two arguments.
*/
template <typename _A, typename _B>
BETAJAEN_SL_INLINE void Each(void (Type::*MethodPtr)(_A, _B), _A _1, _B _2) {

	if (_Flat->_size() == 0)
		return;

	for(Iterator begin = _Flat->First; begin != _Flat->Last; ++begin)
		(*begin->*MethodPtr)(_1, _2);

}


/** \brief For each item in the list call it's own method, with three arguments.
*/
template <typename _A, typename _B, typename _C>
BETAJAEN_SL_INLINE void Each(void (Type::*MethodPtr)(_A, _B, _C), _A _1, _B _2, _C _3) {

	if (_Flat->_size() == 0)
		return;

	for(Iterator begin = _Flat->First; begin != _Flat->Last; ++begin)
		(*begin->*MethodPtr)(_1, _2, _3);

}


/** \brief For each item in the list call it's own method, with four arguments.
*/
template <typename _A, typename _B, typename _C, typename _D>
BETAJAEN_SL_INLINE void Each(void (Type::*MethodPtr)(_A, _B, _C, _D), _A _1, _B _2, _C _3, _D _4) {

	if (_Flat->_size() == 0)
		return;

	for(Iterator begin = _Flat->First; begin != _Flat->Last; ++begin)
		(*begin->*MethodPtr)(_1, _2, _3, _4);

}


/** \brief For each item in the list call it's own method, with five arguments.
*/
template <typename _A, typename _B, typename _C, typename _D, typename _E>
BETAJAEN_SL_INLINE void Each(void (Type::*MethodPtr)(_A, _B, _C, _D), _A _1, _B _2, _C _3, _D _4, _E _5) {

	if (_Flat->_size() == 0)
		return;

	for(Iterator begin = _Flat->First; begin != _Flat->Last; ++begin)
		(*begin->*MethodPtr)(_1, _2, _3, _4, _5);

}


/** \brief For each item in the list call it's own method, with six arguments.
*/
template <typename _A, typename _B, typename _C, typename _D, typename _E, typename _F>
BETAJAEN_SL_INLINE void Each(void (Type::*MethodPtr)(_A, _B, _C, _D), _A _1, _B _2, _C _3, _D _4, _E _5, _F _6) {

	if (_Flat->_size() == 0)
		return;

	for(Iterator begin = _Flat->First; begin != _Flat->Last; ++begin)
		(*begin->*MethodPtr)(_1, _2, _3, _4, _5, _6);

}


/** \brief For each item in the list call it's own method, with seven arguments.
*/
template <typename _A, typename _B, typename _C, typename _D, typename _E, typename _F, typename _G>
BETAJAEN_SL_INLINE void Each(void (Type::*MethodPtr)(_A, _B, _C, _D), _A _1, _B _2, _C _3, _D _4, _E _5, _F _6, _G _7) {

	if (_Flat->_size() == 0)
		return;

	for(Iterator begin = _Flat->First; begin != _Flat->Last; ++begin)
		(*begin->*MethodPtr)(_1, _2, _3, _4, _5, _6, _7);

}


/** \brief For each item in the list call it's own method, with eight arguments.
*/
template <typename _A, typename _B, typename _C, typename _D, typename _E, typename _F, typename _G, typename _H>
BETAJAEN_SL_INLINE void Each(void (Type::*MethodPtr)(_A, _B, _C, _D), _A _1, _B _2, _C _3, _D _4, _E _5, _F _6, _G _7, _H _8) {

	if (_Flat->_size() == 0)
		return;

	for(Iterator begin = _Flat->First; begin != _Flat->Last; ++begin)
		(*begin->*MethodPtr)(_1, _2, _3, _4, _5, _6, _7, _8);

}

/** \brief Returns the pointer to the working set.
*/
BETAJAEN_SL_INLINE FlatPtr getFlat() const {
	return _Flat;
}


/** \brief Returns the number of SharedLists using this set.
*/
BETAJAEN_SL_INLINE unsigned int getUsageCount() const {
	return *_Usage;
}


/** \brief Get an iterator interface for this (and siblings) SharedList
*/
BETAJAEN_SL_INLINE SharedListIteratorType getIterator() {
	return SharedListIteratorType(_Flat);
}

private: unsigned int* _Usage;
private: FlatPtr _Flat;
private: Iterator _Iterator;

};	// End of SharedList<Type, AllocatorType> Class.

BETAJAEN_SL_NAMESPACE_END

/////////////////////////////////////////////////////////////////

#endif

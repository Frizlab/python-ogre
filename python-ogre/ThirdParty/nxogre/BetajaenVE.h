/*

Betajaen's Vector classes

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

#ifndef BETAJAENS_VE_H
#define BETAJAENS_VE_H

// Configuration

/** \brief Namespace to place the Containers in. Comment out if you want it in the global namespace.
*/
#define BETAJAEN_VE_NAMESPACE Betajaen


/** \brief Enable debug comments. Set to zero to disable, 1 to enable
*/
#define BETAJAEN_VE_DEBUG_COMMENTS 1


/** \brief Export Class Macro, replace value with your export macro
*/
#define BETAJAEN_VE_CLASS __declspec(dllexport)
#define BETAJAEN_VE_CLASS_FUNCTION
#define BETAJAEN_VE_TEMPLATE_CLASS __declspec(dllexport)
#define BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION __forceinline

//////////////////////////////////////////////////////////////////////////

#if (BETAJAEN_VE_DEBUG_COMMENTS == 1)
#	include <stdio.h>
#endif 

#ifdef BETAJAEN_VE_NAMESPACE
#   define BETAJAEN_VE_NAMESPACE_START namespace BETAJAEN_VE_NAMESPACE {
#   define BETAJAEN_VE_NAMESPACE_END }
#else
#   define BETAJAEN_VE_NAMESPACE_START /**/
#   define BETAJAEN_VE_NAMESPACE_END /**/
#endif

BETAJAEN_VE_NAMESPACE_START

#if (_MSC_VER>=1000)
#   define BETAJAEN_VE_INLINE __forceinline
#else
#   define BETAJAEN_VE_INLINE inline
#endif

#ifndef BETAJAEN_VE_CLASS
#   define BETAJAEN_VE_CLASS
#endif

#ifndef BETAJAEN_VE_TEMPLATE_CLASS
#   define BETAJAEN_VE_TEMPLATE_CLASS
#endif

#ifndef BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION
#   define BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION
#endif

#ifndef BETAJAEN_VE_DELETE
#   define BETAJAEN_VE_DELETE(ptr) delete ptr;
#endif

#ifndef BETAJAEN_VE_DESTRUCTOR
#   define BETAJAEN_VE_DESTRUCTOR(ptr)
#endif

BETAJAEN_VE_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

BETAJAEN_VE_NAMESPACE_START

template <typename> class Vector2;
template <typename> class Vector3;
template <typename> class Vector4;

BETAJAEN_VE_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

BETAJAEN_VE_NAMESPACE_START

/** \brief Vector with two rows.
*/
template <
			typename Type
		 >
class BETAJAEN_VE_TEMPLATE_CLASS Vector2
{

public: typedef Vector2<Type> ThisType;

/* \brief First */
public: Type i;

/* \brief Second */
public: Type j;

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION Vector2()
{
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION Vector2(Type _i, Type _j) 
: i(_i), j(_j)
{
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION void swap(ThisType& other)
{
	Type First = i, Second = j;
	i = other.i; other.i = First;
	j = other.j; other.j = Second; 
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION ThisType& operator = (const ThisType& other)
{
	i = other.i; j = other.j;
	return *this;
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION ThisType& operator = (const Type& other)
{
	i = other; j = other;
	return *this;
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION ThisType& operator *= (const Type& other)
{
	i *= other; j *= other;
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION ThisType& operator *= (const ThisType& other)
{
	i *= other.i; j *= other.j;
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION bool operator == (const ThisType& other) const
{
	return (i == other.i && j == other.j);
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION bool operator != (const ThisType& other) const
{
	return (i != other.i || j != other.j);
}

};

BETAJAEN_VE_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

BETAJAEN_VE_NAMESPACE_START

/** \brief Vector with three rows.
*/
template <
			typename Type
		 >
class BETAJAEN_VE_TEMPLATE_CLASS Vector3
{

public: typedef Vector3<Type> ThisType;

/* \brief First */
public: Type i;

/* \brief Second */
public: Type j;

/* \brief Third */
public: Type k;


public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION Vector3()
{
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION Vector3(Type _i, Type _j, Type _k) 
: i(_i), j(_j), k(_k)
{
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION void swap(ThisType& other)
{
	Type First = i, Second = j, Third = k;
	i = other.i; other.i = First;
	j = other.j; other.j = Second; 
	k = other.k; other.k = Third;
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION ThisType& operator = (const ThisType& other)
{
	i = other.i; j = other.j; k = other.k;
	return *this;
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION ThisType& operator = (const Type& other)
{
	i = other; j = other; k = other;
	return *this;
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION ThisType& operator *= (const Type& other)
{
	i *= other; j *= other; k *= other;
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION ThisType& operator *= (const ThisType& other)
{
	i *= other.i; j *= other.j; k *= other.k;
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION bool operator == (const ThisType& other) const
{
	return (i == other.i && j == other.j && k == other.k);
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION bool operator != (const ThisType& other) const
{
	return (i != other.i || j != other.j || k != other.k);
}

}; // End of Vector3<Type> class.

BETAJAEN_VE_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

BETAJAEN_VE_NAMESPACE_START

/** \brief Vector with four rows.
*/
template <
			typename Type
		 >
class BETAJAEN_VE_TEMPLATE_CLASS Vector4 {

public: typedef Vector4<Type> ThisType;

/* \brief First */
public: Type i;

/* \brief Second */
public: Type j;

/* \brief Third */
public: Type k;

/* \brief Fourth */
public: Type l;

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION Vector4()
{
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION Vector4(Type _i, Type _j, Type _k, Type _l) 
: i(_i), j(_j), k(_k), l(_l)
{
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION void swap(ThisType& other)
{
	Type First = i, Second = j, Third = k, Fourth = l;
	i = other.i; other.i = First;
	j = other.j; other.j = Second; 
	k = other.k; other.k = Third;
	l = other.l; other.l = Fourth;
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION ThisType& operator = (const ThisType& other) {
	i = other.i; j = other.j; k = other.k; l = other.l;
	return *this;
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION ThisType& operator *= (const Type& other)
{
	i *= other; j *= other; k *= other; l *= other;
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION ThisType& operator *= (const ThisType& other)
{
	i *= other.i; j *= other.j; k *= other.k; l *= other.l;
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION bool operator == (const ThisType& other) const
{
	return (i == other.i && j == other.j && k == other.k && l == other.l);
}

public: BETAJAEN_VE_TEMPLATE_CLASS_FUNCTION bool operator != (const ThisType& other) const
{
	return (i != other.i || j != other.j || k != other.k || l != other.l);
}

};

BETAJAEN_VE_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif
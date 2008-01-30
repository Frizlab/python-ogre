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

#ifndef __NXOGRE_HELPERS_H__
#define __NXOGRE_HELPERS_H__

#include "NxOgrePrerequisites.h"
#include "NxExtended.h"
#include "OgreVector3.h"
#include "OgreQuaternion.h"

namespace NxOgre {

//	NxVec3 NxExport NxConvert<NxVec3, Ogre::Vector3>(const Ogre::Vector3& v);
//	Ogre::Vector3 NxExport NxConvert<Ogre::Vector3, NxVec3>(const NxVec3& v);
//	Ogre::Quaternion NxExport NxConvert<Ogre::Quaternion, NxQuat>(const NxQuat& q);
//	NxQuat NxExport NxConvert<NxQuat, Ogre::Quaternion>(const Ogre::Quaternion& q);
	NxExtendedVec3 NxExport toNxExtendedVec3(const Ogre::Vector3& v);
	NxMat33 toNxMat33(const Ogre::Matrix3&);
	NxMat34 toNxMat34(const Ogre::Matrix4&);
	NxString NxExport NxCreateID(unsigned int groupCount, NxString typeHash);

	////////////////////////////////////////////////////////////////////////////////////////

	void NxExport NxCookTriMeshToDisk(const Ogre::String& meshName);
	void NxExport NxCookConvexToDisk(const Ogre::String& meshName);

	//
	// String Functions
	//


	/** @brief Tokenizes a string into a stl vector of strings based of a delimiter
		
		@param The String to split up
		@param A single character delimiter
		@param Maximum number of splits. 0 is infinite.
		@param Remove excess spaces and tabs before and after each split string.
	
		@example
			<code>
				std::vector<NxString> strings = NxSplitString("1,2,3 , 4", ',', 0, true);
				// Results in:
				//		[0] -> "1"
				//		[1] -> "2"
				//		[2] -> "3"
				//		[3] => "4"
			</code>
		@return The vector of strings
	*/
	std::vector<NxString> NxExport NxStringSplit(const NxString& string, char delimiter, unsigned int maxSplits = 0, bool trim = false);
	

	/** @brief Joins a String together using a seperator and an optional final seperator.

	*/
	template <typename Type>
	NxString NxExport NxStringJoin(std::vector<Type>, const NxString& seperator, const NxString& lastSeperator = "");


	//
	// Operators
	//


	/** @brief Quaternion by NxVec3 work around. Equilvent to NxQuat::rot(NxVec3)
		@example
			<code>
				NxVec3 result = quaternion * vector;
			</code>
		@result	The rotated vector
	*/
	NxVec3 NxExport operator* (const NxQuat&, const NxVec3&);


	// To Strings

	/** @brief Generic class/integral to String.
		@note Uses STL stringstream for conversion.
		@example
			<code>
				NxString str = NxToString<float>(1.2345f);
			</code>
		@result The converted string
	*/
	template <typename Type> NxString NxExport NxToString(const Type& type);


	/** @brief NxVec3 to String, NxVec3(1, -3.3f, 4) => "1 -3.3 4".
		@example
			<code>
				NxString str = NxToString<NxVec3>(NxVec3(1, -3.3, 4));
			</code>
		@result The converted String
	*/
	template <> NxString NxExport NxToString<NxVec3>(const NxVec3& vec3);
	

	/** @brief NxQuat to String, NxQuat(1, 0, 0, 0.7f) => "1 0 0 0.7".
		@example
			<code>
				NxString str = NxToString<NxQuat>(NxQuat(1, 0, 0, 0.7f));
			</code>
		@result The converted String
	*/
	template <> NxString NxExport NxToString<NxQuat>(const NxQuat& quat);


	/** @brief Vector3 to String, NxVec3(1, -3.3f, 4) => "1 -3.3 4".
		@example
			<code>
				NxString str = NxToString<Vector3>(Vector3(1, -3.3, 4));
			</code>
		@result The converted String
	*/
	template <> NxString NxExport NxToString<Ogre::Vector3>(const Ogre::Vector3& vec3);


	/** @brief Quaternion to String, NxQuat(1, 0, 0, 0.7f) => "1 0 0 0.7".
		@example
			<code>
				NxString str = NxToString<Quaternion>(Quaternion(1, 0, 0, 0.7f));
			</code>
		@result The converted String
	*/
	template <> NxString NxExport NxToString<Ogre::Quaternion>(const Ogre::Quaternion& quat);


	//
	// From Strings
	//


	/** @brief String to generic class or intergral from NxString.
		
		@note Converted using the STL stringstream interface.
		
		@example
			<code>
				NxString luggageCombination("12345");
				int number = NxFromString<int>(luggageCombination);
			</code>

		@param String to convert
		
	*/
	template <typename Type> NxExport Type NxFromString(const NxString&);


	/** @brief String to NxVec3. Coordinates seperated by spaces.

		@example
			<code>
				NxVec3 vector = NxFromString<NxVec3>("-10.21 2.324 3");
			</code>

		@result The parsed NxVec3 
	*/
	template <> NxVec3	NxExport NxFromString<NxVec3>(const NxString &);


	/** @brief String to NxQuat. Coordinates seperated by spaces.

		@example
			<code>
				NxQuat quaternion = NxFromString<NxQuat>("1 0 0 0.4");
			</code>

		@result The parsed NxQuat
	*/
	template <> NxQuat	NxExport NxFromString<NxQuat>(const NxString &);


	/** @brief String to Ogre::Vector3. Coordinates seperated by spaces.

		@example
			<code>
				Ogre::Vector3 vector = NxFromString<Ogre::Vector3>("10.21 -2.324 3");
			</code>

		@result The parsed Ogre::Vector3
	*/
	template <>	Ogre::Vector3 NxExport NxFromString<Ogre::Vector3>(const NxString&);
	

	/** @brief String to Ogre::Quaternion. Coordinates seperated by spaces.

		@example
			<code>
				Ogre::Quaternion quaternion = NxFromString<Ogre::Quaternion>("1 0 0 0.4");
			</code>

		@result The parsed Ogre::Quaternion
	*/
	template <>	Ogre::Quaternion NxExport NxFromString<Ogre::Quaternion>(const NxString&);


	//
	// Class Conversions
	//


	/** @brief Generic NxConvert. Do not use.
	*/
	template<typename A, typename B>	NX_INLINE A		NxConvert(const B&);


	/** @brief Ogre::Vector3 to NxVec3
		@example NxVec3 v = NxConvert<NxVec3, Ogre::Vector3>(vector);
	*/
	template<>	NX_INLINE
		NxVec3 NxConvert<NxVec3, Ogre::Vector3>(const Ogre::Vector3& v) {
			return NxVec3(v.x, v.y, v.z);
	}


	/** @brief NxVec3 to Ogre::Vector3
		@example
			<code>
				Ogre::Vector3 v = NxConvert<Ogre::Vector3, NxVec3>(vector);
			</code>
	*/
	template<>	NX_INLINE
		Ogre::Vector3 NxConvert<Ogre::Vector3, NxVec3>(const NxVec3& v) {
			return Ogre::Vector3(v.x, v.y, v.z);
	}

	
	/** @brief Ogre::Quaternion to NxQuat
		@example
			<code>
				NxQuat q = NxConvert<NxQuat, Ogre::Quaternion>(quaternion);
			</code>
	*/
	template<>	NX_INLINE
		NxQuat NxConvert<NxQuat, Ogre::Quaternion>(const Ogre::Quaternion& q) {
			NxQuat rq; rq.setWXYZ(q.w, q.x, q.y, q.z);
			return rq;
	}


	/** @brief NxQuaternion to Ogre::Quaternion
		@example
			<code>
				Ogre::Quaternion q = NxConvert<Ogre::Quaternion, NxQuat>(quaternion);
			</code>
	*/
	template<>	NX_INLINE
		Ogre::Quaternion NxConvert<Ogre::Quaternion, NxQuat>(const NxQuat& q) {
			return Ogre::Quaternion(q.w, q.x, q.y, q.z);
	}

	
	/** @brief	Generates a quick hash based on a string. Collisions are probable
				but it's intended use is for quick string checking or identification.
	*/
	NxHashIdentifier NxExport NxCreateHashFromString(const NxString& str);

	
	/** @brief	Generates a quick short hash based on a string. Collisions are probable
				but it's intended quick identification of Actor types.
	*/
	NxShortHashIdentifier NxExport NxCreateShortHashFromString(const NxString& str);


	/** @brief  Interpolates 
	*/
	inline Pose NxExport NxInterpolate(Pose First, Pose Second, NxReal c = 0.5f);

};

#endif

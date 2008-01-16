//
//	NxOgre a wrapper for the PhysX (formerly Novodex) physics library and the Ogre 3D rendering engine.
//	Copyright (C) 2005 - 2007 Robin Southern and NxOgre.org http://www.nxogre.org
//
//	This library is free software{ you can redistribute it and/or
//	modify it under the terms of the GNU Lesser General Public
//	License as published by the Free Software Foundation{ either
//	version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY{ without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//	Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public
//	License along with this library{ if not, write to the Free Software
//	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "NxOgreStable.h"
#include "NxOgreJSON.h"

#if (NX_USE_JSON == 1)

#include "NxOgreJSONSchema.h"
#include "NxuStream2/NXU_schema.h"
#include "json/json.h"
#include "NxOgrescene.h"
#include <ostream>
#include "NxOgreHelpers.h"
#include "NxOgreContainer.h"
#include "NxOgreWorld.h"
#include "NxOgrescene.h"
#include "NxOgreGroup.h"
#include "NxOgreMaterial.h"

#include "string.h"

namespace NxOgre {


/////////////////////////////////////////////////

void NxJSONCollection::__fetchString(const char *&v, Json::Value& d) {
	mStrings.push_back(d.asString());
	v = mStrings.last->data.c_str();
}

/////////////////////////////////////////////////

void NxJSONCollection::__fetchString(const char *&v,const char *name,Json::Value& d) {
	mStrings.push_back(d[name].asString());
	v = mStrings.last->data.c_str();
}

/////////////////////////////////////////////////

Json::Value NxJSONCollection::__fromNxTri(const NXU::NxTri& t) {
	Json::Value vl;
	vl.append(Json::Value(t.a));
	vl.append(Json::Value(t.b));
	vl.append(Json::Value(t.c));
	return vl;
}

/////////////////////////////////////////////////

Json::Value NxJSONCollection::__fromNxVec3(const NxVec3& v) {
	Json::Value vl;
	vl.append(Json::Value(v.x));
	vl.append(Json::Value(v.y));
	vl.append(Json::Value(v.z));
	return vl;
}

/////////////////////////////////////////////////

Json::Value NxJSONCollection::__fromNxMat33(const NxMat33& v) {

	Json::Value mat33;
	float f[9];
	v.getRowMajor(f);

	for (int i=0;i < 9;i++)
		mat33[i] = Json::Value(f[i]);


	return mat33;

}

/////////////////////////////////////////////////

Json::Value NxJSONCollection::__fromNxMat34(const NxMat34& v) {

	Json::Value mat34;
	float f[9];
	v.M.getRowMajor(f);

	for (int i=0;i < 9;i++)
		mat34[i] = (f[i]);

	
	mat34[9] = v.t.x;
	mat34[10] = v.t.y;
	mat34[11] = v.t.z;

	return mat34;
}

/////////////////////////////////////////////////

NxVec3		NxJSONCollection::__toNxVec3(Json::Value& v) {
	NxVec3 vec;

	if (v.size() != 3)
		return vec;

	vec.x = v[NxU32(0)].asDouble();
	vec.y = v[NxU32(1)].asDouble();
	vec.z = v[NxU32(2)].asDouble();

	return vec;
}

/////////////////////////////////////////////////

NxMat33		NxJSONCollection::__toNxMat33(Json::Value& v) {

	NxMat33 m;

	if (v.size() != 9)
		return m;

	float f[9];
	for (int i=0;i < 9;i++)
		f[i] = v[NxU32(i)].asDouble();

	m.setRowMajor(f);

	return m;

}

/////////////////////////////////////////////////

NxMat34		NxJSONCollection::__toNxMat34(Json::Value& v) {

	NxMat34 m;

	if (v.size() != 12)
		return m;

	float f[9];
	for (int i=0;i < 9;i++)
		f[i] = v[NxU32(i)].asDouble();

	m.M.setRowMajor(f);
	m.t.x = v[NxU32(9)].asDouble();
	m.t.y = v[NxU32(10)].asDouble();
	m.t.z = v[NxU32(11)].asDouble();

	return m;
}

/////////////////////////////////////////////////

NXU::NxTri	NxJSONCollection::__toNxTri(Json::Value& v) {
	NXU::NxTri tri;

	if (v.size() != 3)
		return tri;

	tri.a = v[NxU32(0)].asDouble();
	tri.b = v[NxU32(1)].asDouble();
	tri.c = v[NxU32(2)].asDouble();

	return tri;
}

//////////////////////////////////////////////////////////////////////


}; //End of NxOgre namespace.

#endif

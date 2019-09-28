// Copyright(c) 2013-2019, mCODE A/S
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and /or other materials provided with the distribution.
//  3. Neither the name of the copyright holders nor the
//     names of its contributors may be used to endorse or promote products
//     derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 


#include "pch.h"
#include "PhysXShape.h"
#include "StdChips/MatrixChip.h"
#include "PhysXMaterial.h"
#include "StdChips/Value.h"
#include "PhysXSDK.h"
#include "PhysXConvexMesh.h"
#include "PhysXTriangleMesh.h"



using namespace m3d;


CHIPDESCV1_DEF(PhysXShape, L"PhysX Shape", PHYSXSHAPE_GUID, CHIP_GUID);


PhysXShape::PhysXShape()
{
	CREATE_CHILD(0, MATRIXCHIP_GUID, false, UP, L"Transformation");
	CREATE_CHILD(1, PHYSXMATERIAL_GUID, false, UP, L"Material");
	CREATE_CHILD(2, VALUE_GUID, false, UP, L"Mass");

	_shape = nullptr;
	_mass = 1.0f;
	_type = T_NONE;
}

PhysXShape::~PhysXShape()
{
	PX_RELEASE(_shape)
}

bool PhysXShape::CopyChip(Chip *chip)
{
	PhysXShape *c = dynamic_cast<PhysXShape*>(chip);
	B_RETURN(Chip::CopyChip(c));
	SetType(c->_type);
	return true;
}

bool PhysXShape::LoadChip(DocumentLoader &loader)
{
	B_RETURN(Chip::LoadChip(loader));
	Type type;
	LOAD("type", (unsigned&)type);
	SetType(type);
	return true;
}

bool PhysXShape::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(Chip::SaveChip(saver));
	SAVE("type", (unsigned)_type);
	return true;
}

void PhysXShape::SetType(Type t)
{
	if (_type == t)
		return;

	_type = t;

	switch (_type)
	{
	case T_BOX:
	case T_CAPSULE:
	case T_PLANE:
	case T_SPHERE:
		ClearConnections(3);
		break;
	case T_CONVEX_MESH:
		CREATE_CHILD(3, PHYSXCONVEXMESH_GUID, false, UP, L"Convex Mesh");
		ClearConnections(4);
		break;
	case T_HEIGHT_FIELD:
		ClearConnections();
		break;
	case T_TRIANGLE_MESH:
		CREATE_CHILD(3, PHYSXTRIANGLEMESH_GUID, false, UP, L"Triangle Mesh");
		ClearConnections(4);
		break;
	default:
		ClearConnections();
		break;
	}

	PX_RELEASE(_shape)
}

PxShape *PhysXShape::GetShape()
{
	if (_shape)
		return _shape;

	PhysXSDK *sdk = (PhysXSDK*)engine->GetChipManager()->GetGlobalChip(PHYSXSDK_GUID);

	XMFLOAT3 scaling(1.0f, 1.0f, 1.0f);
	XMFLOAT4 rotation(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT3 translation(0.0f, 0.0f, 0.0f);

	ChildPtr<MatrixChip> ch0 = GetChild(0);
	if (ch0) {
		XMMATRIX m = XMLoadFloat4x4(&ch0->GetMatrix());
		XMVECTOR s, r, t;
		if (!XMMatrixDecompose(&s, &r, &t, m))
			return nullptr;
		XMStoreFloat3(&scaling, s);
		XMStoreFloat4(&rotation, r);
		XMStoreFloat3(&translation, t);
	}
	PxTransform transform(PxVec3(translation.x, translation.y, translation.z), PxQuat(rotation.x, rotation.y, rotation.z, rotation.w));

	PxMaterial *material = nullptr;
	ChildPtr<PhysXMaterial> ch1 = GetChild(1);
	if (ch1)
		material = ch1->GetMaterial();
	
	if (!material) {
		PhysXSDK *sdk = (PhysXSDK*)engine->GetChipManager()->GetGlobalChip(PHYSXSDK_GUID);
		material = sdk->GetDefaultMaterial();
		if (!material)
			return nullptr;
	}

	_mass = 1.0f;
	ChildPtr<Value> chMass = GetChild(2);
	if (chMass)
		_mass = (PxReal)chMass->GetValue();
	_mass = __max(0.0f, _mass);
	
	switch (_type)
	{
	case T_BOX:
		_shape = sdk->GetPhysics()->createShape(PxBoxGeometry(scaling.x, scaling.y, scaling.z), *material); break;
	case T_CAPSULE:
		_shape = sdk->GetPhysics()->createShape(PxCapsuleGeometry(scaling.y, scaling.x * 0.5f), *material); break;
	case T_CONVEX_MESH:
		{
			ChildPtr<PhysXConvexMesh> ch = GetChild(3);
			PxConvexMesh *mesh = ch ? ch->GetConvexMesh() : nullptr;
			if (!mesh)
				return nullptr;
			_shape = sdk->GetPhysics()->createShape(PxConvexMeshGeometry(mesh, PxMeshScale(PxVec3(scaling.x, scaling.y, scaling.z), PxQuat(0.0f, 0.0f, 0.0f, 1.0f))), *material);
			break;
		}
	case T_HEIGHT_FIELD:
		return nullptr;
	case T_PLANE:
		_shape = sdk->GetPhysics()->createShape(PxPlaneGeometry(), *material); break;
	case T_SPHERE:
		_shape = sdk->GetPhysics()->createShape(PxSphereGeometry(scaling.x), *material); break;
	case T_TRIANGLE_MESH:
		{
			ChildPtr<PhysXTriangleMesh> ch = GetChild(3);
			PxTriangleMesh *mesh = ch ? ch->GetTriangleMesh() : nullptr;
			if (!mesh)
				return nullptr;
			_shape = sdk->GetPhysics()->createShape(PxTriangleMeshGeometry(mesh, PxMeshScale(PxVec3(scaling.x, scaling.y, scaling.z), PxQuat(0.0f, 0.0f, 0.0f, 1.0f))), *material);
			break;
		}
	default:
		break;
	}

	if (_shape) {
		_shape->setLocalPose(transform);
	}

	return _shape;
}



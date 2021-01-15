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
#include "PhysXConvexMesh.h"
#include "GraphicsChips/StdGeometry.h"
#include "StdChips/MatrixChip.h"
#include <cooking/PxCooking.h> 
#include "PhysXSDK.h"


using namespace m3d;


CHIPDESCV1_DEF(PhysXConvexMesh, L"PhysX Convex Mesh", PHYSXCONVEXMESH_GUID, CHIP_GUID);


PhysXConvexMesh::PhysXConvexMesh()
{
	CREATE_CHILD(0, STDGEOMETRY_GUID, true, UP, L"Geometries");

	_mesh = nullptr;
}

PhysXConvexMesh::~PhysXConvexMesh()
{
	if (_mesh)
		_mesh->release();
}

bool PhysXConvexMesh::CopyChip(Chip *chip) 
{
	PhysXConvexMesh *c = dynamic_cast<PhysXConvexMesh*>(chip);
	B_RETURN(Chip::CopyChip(c));

	return true;
}

bool PhysXConvexMesh::LoadChip(DocumentLoader &loader) 
{
	B_RETURN(Chip::LoadChip(loader));

	return true;
}

bool PhysXConvexMesh::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(Chip::SaveChip(saver));

	return true;
}

PxConvexMesh *PhysXConvexMesh::GetConvexMesh()
{
	if (_mesh)
		return _mesh;

	List<PxVec3> vertices;

	for (unsigned i = 0; i < GetSubConnectionCount(0); i++) {
		ChildPtr<StdGeometry> ch = GetChild(0, i);
		if (!ch)
			continue;
		const List<XMFLOAT3> &v = ch->GetPositions();
		for (size_t j = 0; j < v.size(); j++)
			vertices.push_back(*(PxVec3*)&v[j]);
	}

	if (vertices.empty())
		return nullptr;

	PxConvexMeshDesc convexDesc;
	convexDesc.points.count = (PxU32)vertices.size();
	convexDesc.points.stride = sizeof(PxVec3);
	convexDesc.points.data = &vertices.front();
	convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

	PhysXSDK *sdk = (PhysXSDK*)engine->GetChipManager()->GetGlobalChip(PHYSXSDK_GUID);
	PxCooking *cooking = sdk->GetCooking();
	if (!cooking)
		return nullptr;
	/*
	PxDefaultMemoryOutputStream buf;
	if(!cooking->cookConvexMesh(convexDesc, buf))
		return nullptr;

	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	_mesh = sdk->GetPhysics()->createConvexMesh(input);
	*/
	return _mesh;
}
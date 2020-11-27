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
#include "PhysXTriangleMesh.h"
#include "GraphicsChips/StdGeometry.h"
#include "StdChips/MatrixChip.h"
#include "PhysXSDK.h"
//#include <PxTkStream.h>
#include <cooking/PxCooking.h> 

using namespace m3d;


CHIPDESCV1_DEF(PhysXTriangleMesh, L"PhysX Triangle Mesh", PHYSXTRIANGLEMESH_GUID, CHIP_GUID);


PhysXTriangleMesh::PhysXTriangleMesh()
{
	CREATE_CHILD(0, STDGEOMETRY_GUID, true, UP, L"Geometries");

	_mesh = nullptr;
}

PhysXTriangleMesh::~PhysXTriangleMesh()
{
	if (_mesh)
		_mesh->release();
}

bool PhysXTriangleMesh::CopyChip(Chip *chip) 
{
	PhysXTriangleMesh *c = dynamic_cast<PhysXTriangleMesh*>(chip);
	B_RETURN(Chip::CopyChip(c));

	return true;
}

bool PhysXTriangleMesh::LoadChip(DocumentLoader &loader) 
{
	B_RETURN(Chip::LoadChip(loader));

	return true;
}

bool PhysXTriangleMesh::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(Chip::SaveChip(saver));

	return true;
}

PxTriangleMesh *PhysXTriangleMesh::GetTriangleMesh()
{
	if (_mesh)
		return _mesh;
/*
	List<PxVec3> vertices;

	for (unsigned i = 0; i < GetSubConnectionCount(0); i++) {
		ChildPtr<StdDrawable> ch = GetChild(0, i);
		if (!ch)
			continue;
		const List<XMFLOAT3> &v = ch->GetPositions();
		for (size_t j = 0; j < v.size(); j++)
			vertices.push_back(*(PxVec3*)&v[j]);
	}

	if (vertices.empty())
		return nullptr;

	PxConvexMeshDesc convexDesc;
	convexDesc.points.count = vertices.size();
	convexDesc.points.stride = sizeof(PxVec3);
	convexDesc.points.data = &vertices.front();
	convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

	PhysXSDK *sdk = (PhysXSDK*)engine->GetChipManager()->GetGlobalChip(PHYSXSDK_GUID);
	PxCooking *cooking = sdk->GetCooking();
	if (!cooking)
		return nullptr;

	PxToolkit::MemoryOutputStream buf;
	if(!cooking->cookConvexMesh(convexDesc, buf))
		return nullptr;

	PxToolkit::MemoryInputData input(buf.getData(), buf.getSize());
	_mesh = sdk->GetPhysics()->createConvexMesh(input);
	*/
	return _mesh;
}
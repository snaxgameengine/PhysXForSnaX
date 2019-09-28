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
#include "PhysXRigidStatic.h"
#include "PhysXScene.h"
#include "PhysXShape.h"
#include "StdChips/MatrixChip.h"
#include "StdChips/Value.h"
#include "StdChips/Vector.h"

using namespace m3d;


CHIPDESCV1_DEF(PhysXRigidStatic, L"PhysX Rigid Static", PHYSXRIGIDSTATIC_GUID, PHYSXRIGIDACTOR_GUID);


PhysXRigidStatic::PhysXRigidStatic()
{
	CREATE_CHILD(0, PHYSXSCENE_GUID, false, BOTH, L"Scene");
	CREATE_CHILD(1, MATRIXCHIP_GUID, false, UP, L"Transform");
	CREATE_CHILD(2, PHYSXSHAPE_GUID, true, UP, L"Shapes");

	_actor = nullptr;
}

PhysXRigidStatic::~PhysXRigidStatic()
{
}

void PhysXRigidStatic::CallChip()
{
	if (Exist()) {
		// Update shapes?
		return;
	}

	PxTransform transform = PxTransform(PxIdentity);
	
	ChildPtr<MatrixChip> chTransform = GetChild(1);
	if (chTransform) {
		transform = PxTransform(PxMat44((float*)&chTransform->GetMatrix()));
	}

	_actor = CreateActor(transform);
}

PxRigidStatic *PhysXRigidStatic::CreateActor(const PxTransform &pose)
{
	ChildPtr<PhysXScene> chScene = GetChild(0);
	if (!chScene)
		return nullptr;

	PxScene *scene = chScene->GetScene();
	if (!scene)
		return nullptr;

	PxRigidStatic *actor = scene->getPhysics().createRigidStatic(pose);
	if (!actor)
		return nullptr;

	chScene->RegisterSceneObject(this);

	List<PxReal> shapeMasses;
	unsigned shapeCount = 0;
	
	for (unsigned i = 0; i < GetSubConnectionCount(2); i++) {
		ChildPtr<PhysXShape> ch2 = GetChild(2, i);
		if (ch2) {
			
			PxShape *shape = ch2->GetShape();
			if (shape) {
				actor->attachShape(*shape);
				PxReal mass = ch2->GetMass();
				shapeMasses.push_back(mass);
			}
		}
	}

	PhysXActorData *data = mmnew PhysXActorData(actor);
	data->globalPose = actor->getGlobalPose();
	actor->userData = data;
	scene->addActor(*actor);

	return actor;
}

void PhysXRigidStatic::DestroyActor() 
{
	if (_actor) {
		if (_actor->getScene())
			((PhysXScene*)_actor->getScene()->userData)->UnregisterSceneObject(this);
		mmdelete((PhysXActorData*)_actor->userData);
		_actor->release();
		_actor = nullptr;
	}
}
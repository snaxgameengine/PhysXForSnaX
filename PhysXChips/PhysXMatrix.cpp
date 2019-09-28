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
#include "PhysXMatrix.h"
#include "PhysXRigidActor.h"
#include "PhysX.h"

using namespace m3d;


CHIPDESCV1_DEF(PhysXMatrix, L"PhysX Matrix", PHYSXMATRIX_GUID, MATRIXCHIP_GUID);


PhysXMatrix::PhysXMatrix()
{
	CREATE_CHILD(0, PHYSXRIGIDACTOR_GUID, false, BOTH, L"Actor");
}

PhysXMatrix::~PhysXMatrix()
{
}

const XMFLOAT4X4 &PhysXMatrix::GetMatrix()
{
	ChildPtr<PhysXRigidActor> chActor = GetChild(0);
	if (!chActor)
		return _matrix = IDENTITY;

	PxRigidActor *actor = chActor->GetRigidActor();
	if (!actor)
		return _matrix = IDENTITY;

	PhysXActorData *data = (PhysXActorData*)actor->userData;

	_matrix = XMFLOAT4X4((float*)&PxMat44(data->globalPose));

	return _matrix;
}

void PhysXMatrix::SetMatrix(const XMFLOAT4X4 &m)
{
	ChildPtr<PhysXRigidActor> chActor = GetChild(0);
	if (!chActor)
		return;

	PxRigidActor *actor = chActor->GetRigidActor();
	if (!actor)
		return;

	PxTransform t = PxTransform((PxMat44&)m);
	if (!t.isSane())
		return;

	_matrix = m;

	if (actor->is<PxRigidDynamic>()) {
		if (actor->is<PxRigidDynamic>()->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC) {
			PhysXActorData *p = (PhysXActorData*)actor->userData;
			assert(p);
			p->kinematicTarget = t;
			p->moveKinematic = true;
			return;
		}
	}
	actor->setGlobalPose(t);
}
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
#include "PhysXRope.h"
#include "PhysXRigidDynamic.h"
#include "PhysXJoint.h"
#include "StdChips/Vector.h"
#include "StdChips/Value.h"
#include "PhysXScene.h"


using namespace m3d;


CHIPDESCV1_DEF(PhysXRope, L"PhysX Rope", PHYSXROPE_GUID, CHIP_GUID);


PhysXRope::PhysXRope()
{
	CREATE_CHILD(0, PHYSXRIGIDDYNAMIC_GUID, false, UP, L"Actor Template");
	CREATE_CHILD(1, PHYSXJOINT_GUID, false, UP, L"Joint Template");
	CREATE_CHILD(2, VECTOR_GUID, false, UP, L"Start Position");
	CREATE_CHILD(3, VECTOR_GUID, false, UP, L"End Position");
	CREATE_CHILD(4, VALUE_GUID, false, UP, L"Number of Elements");
}

PhysXRope::~PhysXRope()
{
}

void PhysXRope::CallChip()
{
	if (_actors.size())
		return;
	assert(_joints.empty());

	ChildPtr<PhysXRigidDynamic> chActor = GetChild(0);
	ChildPtr<PhysXJoint> chJoint = GetChild(1);
	ChildPtr<Vector> chP1 = GetChild(2);
	ChildPtr<Vector> chP2 = GetChild(3);
	ChildPtr<Value> chNoE = GetChild(4);

	if (!(chActor && chJoint && chP1 && chP2 && chNoE))
		return;

	PxVec3 p1 = (const PxVec3&)chP1->GetVector();
	PxVec3 p2 = (const PxVec3&)chP2->GetVector();
	unsigned n = (unsigned)chNoE->GetValue();

	PxVec3 dir = (p2 - p1).getNormalized();
	PxVec3 left = dir.cross(PxVec3(-dir.y, fabs(fabs(dir.z) > 0.577f ? dir.z : dir.x), 0.0f)).getNormalized();
	PxVec3 up = left.cross(dir);
	PxMat33 orientation(dir, up, left);

	float length = (p2 - p1).magnitude();
	float eLength = length / n;

	PxTransform jTransform0(PxVec3(eLength, 0.0f, 0.0f));
	PxTransform jTransform1(PxVec3(0.0f, 0.0f, 0.0f));

	// Note: to make ropes for stable I could try to scale the inertia tensor or increase the mass of the upper (crane wire) elements.

	for (unsigned i = 0; i < n; i++) {
		PxTransform t(PxMat44(orientation, p1 + dir * eLength * (float)i));
		PxRigidDynamic *actor = chActor->CreateActor(t);
		_actors.push_back(actor);
		//actor->setMassSpaceInertiaTensor(actor->getMassSpaceInertiaTensor() * 20);
		if (i > 0) {
			PxJoint *joint = chJoint->CreateJoint(actor->getScene()->getPhysics(), _actors[i - 1], jTransform0, _actors[i], jTransform1);
			_joints.push_back(joint);
		}
		else
			((PhysXScene*)actor->getScene()->userData)->RegisterSceneObject(this);
	}
}

void PhysXRope::DestroyRope()
{
	if (_actors.size()) {
		if (_actors.front()->getScene())
			((PhysXScene*)_actors.front()->getScene()->userData)->UnregisterSceneObject(this);
	}
	for (size_t i = 0; i < _actors.size(); i++) {
		mmdelete((PhysXActorData*)_actors[i]->userData);
		_actors[i]->release();
	}
	_actors.clear();
	for (size_t i = 0; i < _joints.size(); i++) {
		_joints[i]->release();
	}
	_joints.clear();
}
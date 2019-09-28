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
#include "PhysXArticulationLink.h"
#include "PhysXArticulationJointRC.h"
#include "PhysXScene.h"
#include "PhysXShape.h"
#include "StdChips/MatrixChip.h"
#include "StdChips/Value.h"
#include "StdChips/Vector.h"


using namespace m3d;

CHIPDESCV1_DEF(PhysXArticulationLink, L"PhysX Articulation Link", PHYSXARTICULATIONLINK_GUID, PHYSXRIGIDBODY_GUID);


PhysXArticulationLink::PhysXArticulationLink() : _link(nullptr)
{
	CREATE_CHILD(0, MATRIXCHIP_GUID, false, UP, L"Transform");
	CREATE_CHILD(1, VALUE_GUID, false, UP, L"Mass");
	CREATE_CHILD(2, VECTOR_GUID, false, UP, L"Center of Mass");
	CREATE_CHILD(3, PHYSXSHAPE_GUID, true, UP, L"Shapes");
	CREATE_CHILD(4, PHYSXARTICULATIONJOINTBASE_GUID, true, BOTH, L"Child Links");
}

PhysXArticulationLink::~PhysXArticulationLink()
{
	if (_link) {
		PhysXActorData *d = (PhysXActorData*)_link->userData;
		if (d && d->chip == this)
			d->chip = nullptr;
	}
}

PxArticulationLink *PhysXArticulationLink::CreateLink(PxArticulationReducedCoordinate *a, PxArticulationLink *p)
{
	if (_link)
		return nullptr;

	PxTransform transform = PxTransform(PxIdentity);

	ChildPtr<MatrixChip> chTransform = GetChild(0);
	if (chTransform) {
		transform = PxTransform(PxMat44((float*)&chTransform->GetMatrix()));
	}

	PxArticulationLink* link = a->createLink(p, transform);
	if (!link)
		return nullptr;

	// Note: a-links can not be kinematic, no damping or contact report thresholds!
	//	link->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, _disableGravity);
	//	link->setActorFlag(PxActorFlag::eSEND_SLEEP_NOTIFIES, _sendSleepNotifies);

	List<PxReal> shapeMasses;

	for (unsigned i = 0; i < GetSubConnectionCount(3); i++) {
		ChildPtr<PhysXShape> chShape = GetChild(3, i);
		if (chShape) {
			PxShape *shape = chShape->GetShape();
			if (shape) {
				link->attachShape(*shape);
				PxReal mass = chShape->GetMass();
				shapeMasses.push_back(mass);
			}
		}
	}

	float mass = 0.0f;
	ChildPtr<Value> chMass = GetChild(1);
	if (chMass)
		mass = (float)chMass->GetValue();
	mass = __max(mass, 0.0f);

	PxVec3 com(0.0f, 0.0f, 0.0f);
	ChildPtr<Vector> chCOM = GetChild(2);
	if (chCOM) {
		XMFLOAT4 v = chCOM->GetVector();
		com = PxVec3(v.x, v.y, v.z);
	}

	if (mass == 0.0f) {
		bool b = PxRigidBodyExt::setMassAndUpdateInertia(*link, &shapeMasses.front(), (PxU32)shapeMasses.size(), &com);
	}
	else {
		bool b = PxRigidBodyExt::setMassAndUpdateInertia(*link, mass, &com);
	}

	PhysXActorData *data = mmnew PhysXActorData(link);
	data->globalPose = link->getGlobalPose();
	data->chip = this;
	link->userData = data;

	for (size_t i = 0; i < GetSubConnectionCount(4); i++) {
		ChildPtr<PhysXArticulationJointRC> ch4 = GetChild(4, i);
		if (!ch4)
			continue;
		if (ch4->CreateLink(a, link) == nullptr)
			continue;
	}

	return link;
}

void PhysXArticulationLink::ReleaseLink()
{
	_link = nullptr;
}
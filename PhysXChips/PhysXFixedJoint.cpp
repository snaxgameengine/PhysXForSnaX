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
#include "PhysXFixedJoint.h"
#include "PhysXRigidActor.h"
#include "StdChips/MatrixChip.h"

using namespace m3d;


CHIPDESCV1_DEF(PhysXFixedJoint, L"PhysX Fixed Joint", PHYSXFIXEDJOINT_GUID, PHYSXJOINT_GUID);


PhysXFixedJoint::PhysXFixedJoint()
{
	_projectionLinearTolerance = 1.0e10f;
	_projectionAngularTolerance = XM_PI;
}

PhysXFixedJoint::~PhysXFixedJoint()
{
}

bool PhysXFixedJoint::CopyChip(Chip *chip)
{
	PhysXFixedJoint *c = dynamic_cast<PhysXFixedJoint*>(chip);
	B_RETURN(PhysXJoint::CopyChip(c));
	_projectionLinearTolerance = c->_projectionLinearTolerance;
	_projectionAngularTolerance = c->_projectionAngularTolerance;
	return true;
}

bool PhysXFixedJoint::LoadChip(DocumentLoader &loader)
{
	B_RETURN(PhysXJoint::LoadChip(loader));
	LOAD("projectionLinearTolerance", _projectionLinearTolerance);
	LOAD("projectionAngularTolerance", _projectionAngularTolerance);
	return true;
}

bool PhysXFixedJoint::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(PhysXJoint::SaveChip(saver));
	SAVE("projectionLinearTolerance", _projectionLinearTolerance);
	SAVE("projectionAngularTolerance", _projectionAngularTolerance);
	return true;
}

PxJoint *PhysXFixedJoint::CreateJoint(PxPhysics& physics, PxRigidActor* actor0, const PxTransform& localFrame0, PxRigidActor* actor1, const PxTransform& localFrame1)
{
	PxFixedJoint *j = PxFixedJointCreate(physics, actor0, localFrame0, actor1, localFrame1);
	j->setProjectionLinearTolerance(_projectionLinearTolerance);
	j->setProjectionAngularTolerance(_projectionAngularTolerance);
	return j;
}

float PhysXFixedJoint::GetProjectionLinearTolerance() const
{
	PxFixedJoint *j = (PxFixedJoint*)GetJoint();
	if (j)
		return j->getProjectionLinearTolerance();
	return _projectionLinearTolerance;
}

float PhysXFixedJoint::GetProjectionAngularTolerance() const
{
	PxFixedJoint *j = (PxFixedJoint*)GetJoint();
	if (j)
		return j->getProjectionAngularTolerance();
	return _projectionAngularTolerance;
}

void PhysXFixedJoint::SetProjectionLinearTolerance(float t, bool chipSettings)
{
	t = __max(t, 0.0f);
	PxFixedJoint *j = (PxFixedJoint*)GetJoint();
	if (j) 
		j->setProjectionLinearTolerance(t);
	if (chipSettings)
		_projectionLinearTolerance = t;
}

void PhysXFixedJoint::SetProjectionAngularTolerance(float t, bool chipSettings)
{
	t = __min(__max(t, 0.0f), XM_PI);
	PxFixedJoint *j = (PxFixedJoint*)GetJoint();
	if (j) 
		j->setProjectionAngularTolerance(t);
	if (chipSettings)
		_projectionAngularTolerance = t;
}

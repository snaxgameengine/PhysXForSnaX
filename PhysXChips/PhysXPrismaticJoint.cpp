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
#include "PhysXPrismaticJoint.h"
#include "PhysXRigidActor.h"
#include "StdChips/MatrixChip.h"

using namespace m3d;


CHIPDESCV1_DEF(PhysXPrismaticJoint, L"PhysX Prismatic Joint", PHYSXPRISMATICJOINT_GUID, PHYSXJOINT_GUID);


PhysXPrismaticJoint::PhysXPrismaticJoint()
{
	_limitEnable = false;
	_limitLower = -FLT_MAX;
	_limitUpper = FLT_MAX;
	_limitRestitution = 0.0f;
	_limitStiffness = 0.0f;
	_limitDamping = 0.0f;
	_limitContactDistance = 0.01f;
	_projectionLinearTolerance = 1.0e10f;
	_projectionAngularTolerance = XM_PI;
}

PhysXPrismaticJoint::~PhysXPrismaticJoint()
{
}

bool PhysXPrismaticJoint::CopyChip(Chip *chip)
{
	PhysXPrismaticJoint *c = dynamic_cast<PhysXPrismaticJoint*>(chip);
	B_RETURN(PhysXJoint::CopyChip(c));
	_limitEnable = c->_limitEnable;
	_limitLower = c->_limitLower;
	_limitUpper = c->_limitUpper;
	_limitRestitution = c->_limitRestitution;
	_limitStiffness = c->_limitStiffness;
	_limitDamping = c->_limitDamping;
	_limitContactDistance = c->_limitContactDistance;
	_projectionLinearTolerance = c->_projectionLinearTolerance;
	_projectionAngularTolerance = c->_projectionAngularTolerance;
	return true;
}

bool PhysXPrismaticJoint::LoadChip(DocumentLoader &loader)
{
	B_RETURN(PhysXJoint::LoadChip(loader));
	LOADDEF("limitEnable", _limitEnable, false);
	LOADDEF("limitLower", _limitLower, -FLT_MAX);
	LOADDEF("limitUpper", _limitUpper, FLT_MAX);
	LOADDEF("limitRestitution", _limitRestitution, 0.0f);
	LOADDEF("limitStiffness", _limitStiffness, 0.0f);
	LOADDEF("limitDamping", _limitDamping, 0.0f);
	LOADDEF("limitContactDistance", _limitContactDistance, 0.01f);
	LOADDEF("projectionLinearTolerance", _projectionLinearTolerance, 1.0e10f);
	LOADDEF("projectionAngularTolerance", _projectionAngularTolerance, XM_PI);
	return true;
}

bool PhysXPrismaticJoint::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(PhysXJoint::SaveChip(saver));
	SAVEDEF("limitEnable", _limitEnable, false);
	SAVEDEF("limitLower", _limitLower, -FLT_MAX);
	SAVEDEF("limitUpper", _limitUpper, FLT_MAX);
	SAVEDEF("limitRestitution", _limitRestitution, 0.0f);
	SAVEDEF("limitStiffness", _limitStiffness, 0.0f);
	SAVEDEF("limitDamping", _limitDamping, 0.0f);
	SAVEDEF("limitContactDistance", _limitContactDistance, 0.01f);
	SAVEDEF("projectionLinearTolerance", _projectionLinearTolerance, 1.0e10f);
	SAVEDEF("projectionAngularTolerance", _projectionAngularTolerance, XM_PI);
	return true;
}

PxJoint *PhysXPrismaticJoint::CreateJoint(PxPhysics& physics, PxRigidActor* actor0, const PxTransform& localFrame0, PxRigidActor* actor1, const PxTransform& localFrame1)
{
	PxPrismaticJoint *j = PxPrismaticJointCreate(physics, actor0, localFrame0, actor1, localFrame1);
	j->setPrismaticJointFlag(PxPrismaticJointFlag::eLIMIT_ENABLED, _limitEnable);
	PxJointLinearLimitPair l(PxTolerancesScale(), _limitLower, _limitUpper, _limitContactDistance); // TODO: PxTolerancesScale
	l.damping = _limitDamping;
	l.restitution = _limitRestitution;
	l.stiffness = _limitStiffness;
	j->setLimit(l);
	j->setProjectionLinearTolerance(_projectionLinearTolerance);
	j->setProjectionAngularTolerance(_projectionAngularTolerance);
	return j;
}

bool PhysXPrismaticJoint::IsLimitEnabled() const
{
	PxPrismaticJoint *j = (PxPrismaticJoint*)GetJoint();
	if (j)
		return j->getPrismaticJointFlags() == PxPrismaticJointFlag::eLIMIT_ENABLED;
	return _limitEnable;
}

void PhysXPrismaticJoint::SetLimitEnable(bool b, bool chipSettings)
{
	PxPrismaticJoint *j = (PxPrismaticJoint*)GetJoint();
	if (j) 
		j->setPrismaticJointFlag(PxPrismaticJointFlag::eLIMIT_ENABLED, b);
	if (chipSettings)
		_limitEnable = b;
}

void PhysXPrismaticJoint::GetLimitParams(float &lower, float &upper, float &limitRestitution, float &limitStiffness, float &limitDamping, float &limitContactDistance) const
{
	PxPrismaticJoint *j = (PxPrismaticJoint*)GetJoint();
	if (j) {
		PxJointLinearLimitPair &l = j->getLimit();
		lower = l.lower;
		upper = l.upper;
		limitRestitution = l.restitution;
		limitStiffness = l.stiffness;
		limitDamping = l.damping;
		limitContactDistance = l.contactDistance;
		return;
	}
	lower = _limitLower;
	upper = _limitUpper;
	limitRestitution = _limitRestitution;
	limitStiffness = _limitStiffness;
	limitDamping = _limitDamping;
	limitContactDistance = _limitContactDistance;
}

void PhysXPrismaticJoint::SetLimitParams(float lower, float upper, float limitRestitution, float limitStiffness, float limitDamping, float limitContactDistance, bool chipSettings)
{
	upper = __max(lower, upper);
	limitRestitution = __min(__max(limitRestitution, 0.0f), 1.0f);
	limitStiffness = __max(limitStiffness, 0.0f);
	limitDamping = __max(limitDamping, 0.0f);
	limitContactDistance = __max(limitContactDistance, 0.0f);
	PxPrismaticJoint *j = (PxPrismaticJoint*)GetJoint();
	if (j) {
		PxJointLinearLimitPair l(PxTolerancesScale(), lower, upper, limitContactDistance); // TODO: PxTolerancesScale
		l.damping = limitDamping;
		l.restitution = limitRestitution;
		l.stiffness = limitStiffness;
		j->setLimit(l);
		j->setPrismaticJointFlag(PxPrismaticJointFlag::eLIMIT_ENABLED, _limitEnable);
	}
	if (chipSettings) {
		_limitLower = lower;
		_limitUpper = upper;
		_limitRestitution = limitRestitution;
		_limitStiffness = limitStiffness;
		_limitDamping = limitDamping;
		_limitContactDistance = limitContactDistance;
	}
}

float PhysXPrismaticJoint::GetProjectionLinearTolerance() const
{
	PxPrismaticJoint *j = (PxPrismaticJoint*)GetJoint();
	if (j)
		return j->getProjectionLinearTolerance();
	return _projectionLinearTolerance;
}

void PhysXPrismaticJoint::SetProjectionLinearTolerance(float l, bool chipSettings)
{
	l = __max(l, 0.0f);
	PxPrismaticJoint *j = (PxPrismaticJoint*)GetJoint();
	if (j) 
		j->setProjectionLinearTolerance(l);
	if (chipSettings)
		_projectionLinearTolerance = l;
}

float PhysXPrismaticJoint::GetProjectionAngularTolerance() const
{
	PxPrismaticJoint *j = (PxPrismaticJoint*)GetJoint();
	if (j)
		return j->getProjectionAngularTolerance();
	return _projectionAngularTolerance;
}

void PhysXPrismaticJoint::SetProjectionAngularTolerance(float l, bool chipSettings)	
{
	l = __min(__max(l, 0.0f), XM_PI);
	PxPrismaticJoint *j = (PxPrismaticJoint*)GetJoint();
	if (j) 
		j->setProjectionAngularTolerance(l);
	if (chipSettings)
		_projectionAngularTolerance = l;
}




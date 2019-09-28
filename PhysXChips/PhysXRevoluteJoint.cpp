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
#include "PhysXRevoluteJoint.h"
#include "PhysXRigidActor.h"
#include "StdChips/MatrixChip.h"

using namespace m3d;


CHIPDESCV1_DEF(PhysXRevoluteJoint, L"PhysX Revolute Joint", PHYSXREVOLUTEJOINT_GUID, PHYSXJOINT_GUID);


PhysXRevoluteJoint::PhysXRevoluteJoint()
{
	_limitEnable = false;
	_limitLower = -XM_PIDIV2;
	_limitUpper = XM_PIDIV2;
	_limitRestitution = 0.0f;
	_limitStiffness = 0.0f;
	_limitDamping = 0.0f; 
	_limitContactDistance = 0.05f;
	_driveEnable = false;
	_driveFreespin = true;
	_driveVelocity = 0.0f;
	_driveForceLimit = FLT_MAX;
	_driveGearRatio = 1.0f;
	_projectionLinearTolerance = 1.0e10f;
	_projectionAngularTolerance = XM_PI;
}

PhysXRevoluteJoint::~PhysXRevoluteJoint()
{
}

bool PhysXRevoluteJoint::CopyChip(Chip *chip)
{
	PhysXRevoluteJoint *c = dynamic_cast<PhysXRevoluteJoint*>(chip);
	B_RETURN(PhysXJoint::CopyChip(c));
	_limitEnable = c->_limitEnable;
	_limitLower = c->_limitLower;
	_limitUpper = c->_limitUpper;
	_limitRestitution = c->_limitRestitution;
	_limitStiffness = c->_limitStiffness;
	_limitDamping = c->_limitDamping;
	_limitContactDistance = c->_limitContactDistance;
	_driveEnable = c->_driveEnable;
	_driveFreespin = c->_driveFreespin;
	_driveVelocity = c->_driveVelocity;
	_driveForceLimit = c->_driveForceLimit;
	_driveGearRatio = c->_driveGearRatio;
	_projectionLinearTolerance = c->_projectionLinearTolerance;
	_projectionAngularTolerance = c->_projectionAngularTolerance;
	return true;
}

bool PhysXRevoluteJoint::LoadChip(DocumentLoader &loader)
{
	B_RETURN(PhysXJoint::LoadChip(loader));
	LOADDEF("limitEnable", _limitEnable, false);
	LOADDEF("limitLower", _limitLower, -XM_PIDIV2);
	LOADDEF("limitUpper", _limitUpper, XM_PIDIV2);
	LOADDEF("limitRestitution", _limitRestitution, 0.0f);
	LOADDEF("limitStiffness", _limitStiffness, 0.0f);
	LOADDEF("limitDamping", _limitDamping, 0.0f);
	LOADDEF("limitContactDistance", _limitContactDistance, 0.05f);
	LOADDEF("driveEnable", _driveEnable, false);
	LOADDEF("driveFreespin", _driveFreespin, true);
	LOADDEF("driveVelocity", _driveVelocity, 0.0f);
	LOADDEF("driveForceLimit", _driveForceLimit, FLT_MAX);
	LOADDEF("driveGearRatio", _driveGearRatio, 1.0f);
	LOADDEF("projectionLinearTolerance", _projectionLinearTolerance, 1.0e10f);
	LOADDEF("projectionAngularTolerance", _projectionAngularTolerance, XM_PI);
	return true;
}

bool PhysXRevoluteJoint::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(PhysXJoint::SaveChip(saver));
	SAVEDEF("limitEnable", _limitEnable, false);
	SAVEDEF("limitLower", _limitLower, -XM_PIDIV2);
	SAVEDEF("limitUpper", _limitUpper, XM_PIDIV2);
	SAVEDEF("limitRestitution", _limitRestitution, 0.0f);
	SAVEDEF("limitStiffness", _limitStiffness, 0.0f);
	SAVEDEF("limitDamping", _limitDamping, 0.0f);
	SAVEDEF("limitContactDistance", _limitContactDistance, 0.05f);
	SAVEDEF("driveEnable", _driveEnable, false);
	SAVEDEF("driveFreespin", _driveFreespin, true);
	SAVEDEF("driveVelocity", _driveVelocity, 0.05);
	SAVEDEF("driveForceLimit", _driveForceLimit, FLT_MAX);
	SAVEDEF("driveGearRatio", _driveGearRatio, 1.0f);
	SAVEDEF("projectionLinearTolerance", _projectionLinearTolerance, 1.0e10f);
	SAVEDEF("projectionAngularTolerance", _projectionAngularTolerance, XM_PI);
	return true;
}

PxJoint *PhysXRevoluteJoint::CreateJoint(PxPhysics& physics, PxRigidActor* actor0, const PxTransform& localFrame0, PxRigidActor* actor1, const PxTransform& localFrame1)
{
	PxRevoluteJoint *j = PxRevoluteJointCreate(physics, actor0, localFrame0, actor1, localFrame1);
	j->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, _limitEnable);
	j->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, _driveEnable);
	j->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_FREESPIN, _driveFreespin);
	PxJointAngularLimitPair l(_limitLower, _limitUpper, _limitContactDistance);
	l.damping = _limitDamping;
	l.restitution = _limitRestitution;
	l.stiffness = _limitStiffness;
	j->setLimit(l);
	j->setDriveVelocity(_driveVelocity);
	j->setDriveForceLimit(_driveForceLimit);
	j->setDriveGearRatio(_driveGearRatio);
	j->setProjectionLinearTolerance(_projectionLinearTolerance);
	j->setProjectionAngularTolerance(_projectionAngularTolerance);
	return j;
}

bool PhysXRevoluteJoint::IsLimitEnabled() const
{
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j)
		return j->getRevoluteJointFlags() == PxRevoluteJointFlag::eLIMIT_ENABLED;
	return _limitEnable;
}

void PhysXRevoluteJoint::SetLimitEnable(bool b, bool chipSettings)
{
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j) 
		j->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, b);
	if (chipSettings)
		_limitEnable = b;
}

void PhysXRevoluteJoint::GetLimitParams(float &lower, float &upper, float &limitRestitution, float &limitStiffness, float &limitDamping, float &limitContactDistance) const
{
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j) {
		PxJointAngularLimitPair &l = j->getLimit();
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

void PhysXRevoluteJoint::SetLimitParams(float lower, float upper, float limitRestitution, float limitStiffness, float limitDamping, float limitContactDistance, bool chipSettings)
{
	lower = __min(__max(lower, -XM_PIDIV2), XM_PIDIV2);
	upper = __min(__max(lower, upper), XM_PIDIV2);
	limitRestitution = __min(__max(limitRestitution, 0.0f), 1.0f);
	limitStiffness = __max(limitStiffness, 0.0f);
	limitDamping = __max(limitDamping, 0.0f);
	limitContactDistance = __max(limitContactDistance, 0.0f);
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j) {
		PxJointAngularLimitPair l(lower, upper, limitContactDistance);
		l.damping = limitDamping;
		l.restitution = limitRestitution;
		l.stiffness = limitStiffness;
		j->setLimit(l);
		j->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, _limitEnable);
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

bool PhysXRevoluteJoint::IsDriveEnabled() const
{
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j)
		return j->getRevoluteJointFlags() == PxRevoluteJointFlag::eDRIVE_ENABLED;
	return _driveEnable;
}

void PhysXRevoluteJoint::SetDriveEnable(bool b, bool chipSettings)
{
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j) 
		j->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, b);
	if (chipSettings)
		_driveEnable = b;
}

bool PhysXRevoluteJoint::IsDriveFreespin() const
{
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j)
		return j->getRevoluteJointFlags() == PxRevoluteJointFlag::eDRIVE_FREESPIN;
	return _driveFreespin;
}

void PhysXRevoluteJoint::SetDriveFreespin(bool b, bool chipSettings)
{
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j) 
		j->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_FREESPIN, b);
	if (chipSettings)
		_driveFreespin = b;
}

float PhysXRevoluteJoint::GetDriveVelocity() const
{
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j)
		return j->getDriveVelocity();
	return _driveVelocity;
}

void PhysXRevoluteJoint::SetDriveVelocity(float l, bool chipSettings)
{
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j) 
		j->setDriveVelocity(l);
	if (chipSettings)
		_driveVelocity = l;
}

float PhysXRevoluteJoint::GetDriveForceLimit() const
{
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j)
		return j->getDriveForceLimit();
	return _driveForceLimit;
}

void PhysXRevoluteJoint::SetDriveForceLimit(float l, bool chipSettings)
{
	l = __max(l, 0.0f);
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j) 
		j->setDriveForceLimit(l);
	if (chipSettings)
		_driveForceLimit = l;
}

float PhysXRevoluteJoint::GetDriveGearRatio() const
{
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j)
		return j->getDriveGearRatio();
	return _driveGearRatio;
}

void PhysXRevoluteJoint::SetDriveGearRatio(float l, bool chipSettings)
{
	l = __max(l, 0.0f);
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j) 
		j->setDriveGearRatio(l);
	if (chipSettings)
		_driveGearRatio = l;
}

float PhysXRevoluteJoint::GetProjectionLinearTolerance() const
{
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j)
		return j->getProjectionLinearTolerance();
	return _projectionLinearTolerance;
}

void PhysXRevoluteJoint::SetProjectionLinearTolerance(float l, bool chipSettings)
{
	l = __max(l, 0.0f);
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j) 
		j->setProjectionLinearTolerance(l);
	if (chipSettings)
		_projectionLinearTolerance = l;
}

float PhysXRevoluteJoint::GetProjectionAngularTolerance() const
{
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j)
		return j->getProjectionAngularTolerance();
	return _projectionAngularTolerance;
}

void PhysXRevoluteJoint::SetProjectionAngularTolerance(float l, bool chipSettings)	
{
	l = __min(__max(l, 0.0f), XM_PI);
	PxRevoluteJoint *j = (PxRevoluteJoint*)GetJoint();
	if (j) 
		j->setProjectionAngularTolerance(l);
	if (chipSettings)
		_projectionAngularTolerance = l;
}


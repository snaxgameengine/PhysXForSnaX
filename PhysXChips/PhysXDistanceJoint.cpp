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
#include "PhysXDistanceJoint.h"
#include "PhysXRigidActor.h"
#include "StdChips/MatrixChip.h"

using namespace m3d;


CHIPDESCV1_DEF(PhysXDistanceJoint, L"PhysX Distance Joint", PHYSXDISTANCEJOINT_GUID, PHYSXJOINT_GUID);


PhysXDistanceJoint::PhysXDistanceJoint()
{
	_minDistanceEnable = false;
	_maxDistanceEnable = true;
	_minDistance = 0.0f;
	_maxDistance = 0.0f;
	_tolerance = 0.0f;
	_springEnable = false;
	_stiffness = 0.0f;
	_damping = 0.0f;
}

PhysXDistanceJoint::~PhysXDistanceJoint()
{
}

bool PhysXDistanceJoint::CopyChip(Chip *chip)
{
	PhysXDistanceJoint *c = dynamic_cast<PhysXDistanceJoint*>(chip);
	B_RETURN(PhysXJoint::CopyChip(c));
	_minDistanceEnable = c->_minDistanceEnable;
	_maxDistanceEnable = c->_maxDistanceEnable;
	_minDistance = c->_minDistance;
	_maxDistance = c->_maxDistance;
	_tolerance = c->_tolerance;
	_springEnable = c->_springEnable;
	_stiffness = c->_stiffness;
	_damping = c->_damping;
	return true;
}

bool PhysXDistanceJoint::LoadChip(DocumentLoader &loader)
{
	B_RETURN(PhysXJoint::LoadChip(loader));
	LOADDEF("minDistanceEnable", _minDistanceEnable, false);
	LOADDEF("maxDistanceEnable", _maxDistanceEnable, true);
	LOADDEF("minDistance", _minDistance, 0.0f);
	LOADDEF("maxDistance", _maxDistance, 0.0f);
	LOADDEF("tolerance", _tolerance, 0.0f);
	LOADDEF("springEnable", _springEnable, false);
	LOADDEF("stiffness", _stiffness, 0.0f);
	LOADDEF("damping", _damping, 0.0f);
	return true;
}

bool PhysXDistanceJoint::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(PhysXJoint::SaveChip(saver));
	SAVEDEF("minDistanceEnable", _minDistanceEnable, false);
	SAVEDEF("maxDistanceEnable", _maxDistanceEnable, true);
	SAVEDEF("minDistance", _minDistance, 0.0f);
	SAVEDEF("maxDistance", _maxDistance, 0.0f);
	SAVEDEF("tolerance", _tolerance, 0.0f);
	SAVEDEF("springEnable", _springEnable, false);
	SAVEDEF("stiffness", _stiffness, 0.0f);
	SAVEDEF("damping", _damping, 0.0f);
	return true;
}

PxJoint *PhysXDistanceJoint::CreateJoint(PxPhysics& physics, PxRigidActor* actor0, const PxTransform& localFrame0, PxRigidActor* actor1, const PxTransform& localFrame1)
{
	PxDistanceJoint *j = PxDistanceJointCreate(physics, actor0, localFrame0, actor1, localFrame1);

	j->setDistanceJointFlag(PxDistanceJointFlag::eMIN_DISTANCE_ENABLED, _minDistanceEnable);
	j->setDistanceJointFlag(PxDistanceJointFlag::eMAX_DISTANCE_ENABLED, _maxDistanceEnable);
	j->setMinDistance(_minDistance);
	j->setMaxDistance(_maxDistance);
	j->setTolerance(_tolerance);
	j->setDistanceJointFlag(PxDistanceJointFlag::eSPRING_ENABLED, _springEnable);
	j->setStiffness(_stiffness);
	j->setDamping(_damping);

	return j;
}

bool PhysXDistanceJoint::IsMinDistanceEnabled() const
{
	PxDistanceJoint *j = (PxDistanceJoint*)GetJoint();
	if (j)
		return j->getDistanceJointFlags() == PxDistanceJointFlag::eMIN_DISTANCE_ENABLED;
	return _minDistanceEnable;
}

bool PhysXDistanceJoint::IsMaxDistanceEnabled() const
{
	PxDistanceJoint *j = (PxDistanceJoint*)GetJoint();
	if (j)
		return j->getDistanceJointFlags() == PxDistanceJointFlag::eMAX_DISTANCE_ENABLED;
	return _maxDistanceEnable;
}

float PhysXDistanceJoint::GetMinDistance() const
{
	PxDistanceJoint *j = (PxDistanceJoint*)GetJoint();
	if (j)
		return j->getMinDistance();
	return _minDistance;
}

float PhysXDistanceJoint::GetMaxDistance() const
{
	PxDistanceJoint *j = (PxDistanceJoint*)GetJoint();
	if (j)
		return j->getMaxDistance();
	return _maxDistance;
}

float PhysXDistanceJoint::GetTolerance() const
{
	PxDistanceJoint *j = (PxDistanceJoint*)GetJoint();
	if (j)
		return j->getTolerance();
	return _tolerance;
}

bool PhysXDistanceJoint::IsSpringEnabled() const
{
	PxDistanceJoint *j = (PxDistanceJoint*)GetJoint();
	if (j)
		return j->getDistanceJointFlags() == PxDistanceJointFlag::eSPRING_ENABLED;
	return _springEnable;
}

float PhysXDistanceJoint::GetStiffness() const
{
	PxDistanceJoint *j = (PxDistanceJoint*)GetJoint();
	if (j)
		return j->getStiffness();
	return _stiffness;
}

float PhysXDistanceJoint::GetDamping() const
{
	PxDistanceJoint *j = (PxDistanceJoint*)GetJoint();
	if (j)
		return j->getDamping();
	return _damping;
}

void PhysXDistanceJoint::SetMinDistanceEnabled(bool b, bool chipSettings)
{
	PxDistanceJoint *j = (PxDistanceJoint*)GetJoint();
	if (j) 
		j->setDistanceJointFlag(PxDistanceJointFlag::eMIN_DISTANCE_ENABLED, b);
	if (chipSettings)
		_minDistanceEnable = b;
}

void PhysXDistanceJoint::SetMaxDistanceEnabled(bool b, bool chipSettings)
{
	PxDistanceJoint *j = (PxDistanceJoint*)GetJoint();
	if (j) 
		j->setDistanceJointFlag(PxDistanceJointFlag::eMAX_DISTANCE_ENABLED, b);
	if (chipSettings)
		_maxDistanceEnable = b;
}

void PhysXDistanceJoint::SetDistanceRange(float minD, float maxD, bool chipSettings)
{
	minD = __max(minD, 0.0f);
	maxD = __max(maxD, minD);
	PxDistanceJoint *j = (PxDistanceJoint*)GetJoint();
	if (j) {
		j->setMinDistance(minD);
		j->setMinDistance(maxD);
	}
	if (chipSettings) {
		_minDistance = minD;
		_maxDistance = maxD;
	}
}

void PhysXDistanceJoint::SetTolerance(float d, bool chipSettings)
{
	d = __max(d, 0.0f);
	PxDistanceJoint *j = (PxDistanceJoint*)GetJoint();
	if (j) 
		j->setTolerance(d);
	if (chipSettings)
		_tolerance = d;
}

void PhysXDistanceJoint::SetSpringEnabled(bool b, bool chipSettings)
{
	PxDistanceJoint *j = (PxDistanceJoint*)GetJoint();
	if (j) 
		j->setDistanceJointFlag(PxDistanceJointFlag::eSPRING_ENABLED, b);
	if (chipSettings)
		_springEnable = b;
}

void PhysXDistanceJoint::SetStiffness(float d, bool chipSettings)
{
	d = __max(d, 0.0f);
	PxDistanceJoint *j = (PxDistanceJoint*)GetJoint();
	if (j) 
		j->setStiffness(d);
	if (chipSettings)
		_stiffness = d;
}

void PhysXDistanceJoint::SetDamping(float d, bool chipSettings)
{
	d = __max(d, 0.0f);
	PxDistanceJoint *j = (PxDistanceJoint*)GetJoint();
	if (j) 
		j->setDamping(d);
	if (chipSettings)
		_damping = d;
}

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
#include "PhysXSphericalJoint.h"
#include "PhysXRigidActor.h"
#include "StdChips/MatrixChip.h"

using namespace m3d;


CHIPDESCV1_DEF(PhysXSphericalJoint, L"PhysX Spherical Joint", PHYSXSPHERICALJOINT_GUID, PHYSXJOINT_GUID);


PhysXSphericalJoint::PhysXSphericalJoint()
{
	_limitEnable = false;
	_yLimitAngle = XM_PIDIV2;
	_zLimitAngle = XM_PIDIV2;
	_limitRestitution = 0.0f;
	_limitStiffness = 0.0f;
	_limitDamping = 0.0f; 
	_limitContactDistance = 0.05f;
	_projectionLinearTolerance = 1.0e10f;
}

PhysXSphericalJoint::~PhysXSphericalJoint()
{
}

bool PhysXSphericalJoint::CopyChip(Chip *chip)
{
	PhysXSphericalJoint *c = dynamic_cast<PhysXSphericalJoint*>(chip);
	B_RETURN(PhysXJoint::CopyChip(c));
	_limitEnable = c->_limitEnable;
	_yLimitAngle = c->_yLimitAngle;
	_zLimitAngle = c->_zLimitAngle;
	_limitRestitution = c->_limitRestitution;
	_limitStiffness = c->_limitStiffness;
	_limitDamping = c->_limitDamping;
	_limitContactDistance = c->_limitContactDistance;
	_projectionLinearTolerance = c->_projectionLinearTolerance;
	return true;
}

bool PhysXSphericalJoint::LoadChip(DocumentLoader &loader)
{
	B_RETURN(PhysXJoint::LoadChip(loader));
	LOADDEF("limitEnable", _limitEnable, false);
	LOADDEF("yLimitAngle", _yLimitAngle, XM_PIDIV2);
	LOADDEF("zLimitAngle", _zLimitAngle, XM_PIDIV2);
	LOADDEF("limitRestitution", _limitRestitution, 0.0f);
	LOADDEF("limitStiffness", _limitStiffness, 0.0f);
	LOADDEF("limitDamping", _limitDamping, 0.0f);
	LOADDEF("limitContactDistance", _limitContactDistance, 0.05f);
	LOADDEF("projectionLinearTolerance", _projectionLinearTolerance, 1.0e10f);
	return true;
}

bool PhysXSphericalJoint::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(PhysXJoint::SaveChip(saver));
	SAVEDEF("limitEnable", _limitEnable, false);
	SAVEDEF("yLimitAngle", _yLimitAngle, XM_PIDIV2);
	SAVEDEF("zLimitAngle", _zLimitAngle, XM_PIDIV2);
	SAVEDEF("limitRestitution", _limitRestitution, 0.0f);
	SAVEDEF("limitStiffness", _limitStiffness, 0.0f);
	SAVEDEF("limitDamping", _limitDamping, 0.0f);
	SAVEDEF("limitContactDistance", _limitContactDistance, 0.05f);
	SAVEDEF("projectionLinearTolerance", _projectionLinearTolerance, 1.0e10f);
	return true;
}

PxJoint *PhysXSphericalJoint::CreateJoint(PxPhysics& physics, PxRigidActor* actor0, const PxTransform& localFrame0, PxRigidActor* actor1, const PxTransform& localFrame1)
{
	PxSphericalJoint *j = PxSphericalJointCreate(physics, actor0, localFrame0, actor1, localFrame1);
	j->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, _limitEnable);
	PxJointLimitCone l(_yLimitAngle, _zLimitAngle, _limitContactDistance);
	l.damping = _limitDamping;
	l.restitution = _limitRestitution;
	l.stiffness = _limitStiffness;
	j->setLimitCone(l);
	j->setProjectionLinearTolerance(_projectionLinearTolerance);
	return j;
}

bool PhysXSphericalJoint::IsLimitEnabled() const
{
	PxSphericalJoint *j = (PxSphericalJoint*)GetJoint();
	if (j)
		return j->getSphericalJointFlags() == PxSphericalJointFlag::eLIMIT_ENABLED;
	return _limitEnable;
}

void PhysXSphericalJoint::SetLimitEnable(bool b, bool chipSettings)
{
	PxSphericalJoint *j = (PxSphericalJoint*)GetJoint();
	if (j) 
		j->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, b);
	if (chipSettings)
		_limitEnable = b;
}

void PhysXSphericalJoint::GetLimitParams(float &yLimitAngle, float &zLimitAngle, float &limitRestitution, float &limitStiffness, float &limitDamping, float &limitContactDistance) const
{
	PxSphericalJoint *j = (PxSphericalJoint*)GetJoint();
	if (j) {
		PxJointLimitCone &l = j->getLimitCone();
		yLimitAngle = l.yAngle;
		zLimitAngle = l.zAngle;
		limitRestitution = l.restitution;
		limitStiffness = l.stiffness;
		limitDamping = l.damping;
		limitContactDistance = l.contactDistance;
		return;
	}
	yLimitAngle = _yLimitAngle;
	zLimitAngle = _zLimitAngle;
	limitRestitution = _limitRestitution;
	limitStiffness = _limitStiffness;
	limitDamping = _limitDamping;
	limitContactDistance = _limitContactDistance;
}

void PhysXSphericalJoint::SetLimitParams(float yLimitAngle, float zLimitAngle, float limitRestitution, float limitSpring, float limitDamping, float limitContactDistance, bool chipSettings)
{
	yLimitAngle = __min(__max(yLimitAngle, 0.01f), XM_PI - 0.01f);
	zLimitAngle = __min(__max(zLimitAngle, 0.01f), XM_PI - 0.01f);
	limitRestitution = __min(__max(limitRestitution, 0.0f), 1.0f);
	limitSpring = __max(limitSpring, 0.0f);
	limitDamping = __max(limitDamping, 0.0f);
	limitContactDistance = __max(limitContactDistance, 0.0f);
	PxSphericalJoint *j = (PxSphericalJoint*)GetJoint();
	if (j) {
		PxJointLimitCone l(yLimitAngle, zLimitAngle, limitContactDistance);
		l.damping = limitDamping;
		l.restitution = limitRestitution;
		l.stiffness = limitSpring;
		j->setLimitCone(l);
		j->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, _limitEnable); // Need this for joint to update!
	}
	if (chipSettings) {
		_yLimitAngle = yLimitAngle;
		_zLimitAngle = zLimitAngle;
		_limitRestitution = limitRestitution;
		_limitStiffness = limitSpring;
		_limitDamping = limitDamping;
		_limitContactDistance = limitContactDistance;
	}
}

float PhysXSphericalJoint::GetProjectionLinearTolerance() const
{
	PxSphericalJoint *j = (PxSphericalJoint*)GetJoint();
	if (j)
		return j->getProjectionLinearTolerance();
	return _projectionLinearTolerance;
}

void PhysXSphericalJoint::SetProjectionLinearTolerance(float l, bool chipSettings)
{
	l = __max(l, 0.0f);
	PxSphericalJoint *j = (PxSphericalJoint*)GetJoint();
	if (j) 
		j->setProjectionLinearTolerance(l);
	if (chipSettings)
		_projectionLinearTolerance = l;
}





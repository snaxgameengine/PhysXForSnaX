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
#include "PhysXD6Joint.h"
#include "PhysXRigidActor.h"
#include "StdChips/MatrixChip.h"

using namespace m3d;


CHIPDESCV1_DEF(PhysXD6Joint, L"PhysX D6 Joint", PHYSXD6JOINT_GUID, PHYSXJOINT_GUID);


PhysXD6Joint::PhysXD6Joint()
{
	_motion[PxD6Axis::eX] = PxD6Motion::eLOCKED;
	_motion[PxD6Axis::eY] = PxD6Motion::eLOCKED;
	_motion[PxD6Axis::eZ] = PxD6Motion::eLOCKED;
	_motion[PxD6Axis::eTWIST] = PxD6Motion::eLOCKED;
	_motion[PxD6Axis::eSWING1] = PxD6Motion::eLOCKED;
	_motion[PxD6Axis::eSWING2] = PxD6Motion::eLOCKED;
/*	_linearLimit.contactDistance = 0.05f;
	_linearLimit.damping = 0.0f;
	_linearLimit.restitution = 0.0f;
	_linearLimit.spring = 0.0f;
	_linearLimit.value = 0.0f;
	_twistLimit.contactDistance = 0.05f;
	_twistLimit.damping = 0.0f;
	_twistLimit.restitution = 0.0f;
	_twistLimit.spring = 0.0f;
	_twistLimit.lower = -XM_PIDIV2;
	_twistLimit.upper = XM_PIDIV2;
	_swingLimit.contactDistance = 0.05f;
	_swingLimit.damping = 0.0f;
	_swingLimit.restitution = 0.0f;
	_swingLimit.spring = 0.0f;
	_swingLimit.yAngle = XM_PIDIV2;
	_swingLimit.zAngle = XM_PIDIV2;
*/
	_projectionLinearTolerance = 1.0e10f;
	_projectionAngularTolerance = XM_PI;
}

PhysXD6Joint::~PhysXD6Joint()
{
}

bool PhysXD6Joint::CopyChip(Chip *chip)
{
	PhysXD6Joint *c = dynamic_cast<PhysXD6Joint*>(chip);
	B_RETURN(PhysXJoint::CopyChip(c));

	return true;
}

bool PhysXD6Joint::LoadChip(DocumentLoader &loader)
{
	B_RETURN(PhysXJoint::LoadChip(loader));

	return true;
}

bool PhysXD6Joint::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(PhysXJoint::SaveChip(saver));

	return true;
}

PxJoint *PhysXD6Joint::CreateJoint(PxPhysics& physics, PxRigidActor* actor0, const PxTransform& localFrame0, PxRigidActor* actor1, const PxTransform& localFrame1)
{
	PxD6Joint *j = PxD6JointCreate(physics, actor0, localFrame0, actor1, localFrame1);


	return j;
}











/*
	PxD6Motion _motionY;
	PxD6Motion _motionZ; 
	PxD6Motion _motionTwist;
	PxD6Motion _motionSwing1; 
	PxD6Motion _motionSwing2; 

	float _linearLimitExtent;
	float _linearLimitRestitution;
	float _linearLimitSpring;
	float _linearLimitDamping; 
	float _linearLimitContactDistance;

	float _twistLimitLower;
	float _twistLimitUpper;
	float _twistLimitRestitution;
	float _twistLimitSpring;
	float _twistLimitDamping; 
	float _twistLimitContactDistance;

	float _swingLimitYAngle;
	float _swingLimitZAngle;
	float _swingLimitRestitution;
	float _swingLimitSpring;
	float _swingLimitDamping; 
	float _swingLimitContactDistance;
*/
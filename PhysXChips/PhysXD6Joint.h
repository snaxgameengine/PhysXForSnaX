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
#pragma once

#include "Exports.h"
#include "PhysXJoint.h"

namespace m3d
{


static const GUID PHYSXD6JOINT_GUID = { 0xfe4a1d1e, 0x461c, 0x4c0f, { 0xa8, 0x8c, 0xe4, 0x88, 0x62, 0x10, 0x8a, 0xd5 } };



class PHYSXCHIPS_API PhysXD6Joint : public PhysXJoint
{
	CHIPDESC_DECL;
public:
	PhysXD6Joint();
	virtual ~PhysXD6Joint();

	virtual bool CopyChip(Chip *chip) override;
	virtual bool LoadChip(DocumentLoader &loader) override;
	virtual bool SaveChip(DocumentSaver &saver) const override;

	virtual PxJoint *CreateJoint(PxPhysics& physics, PxRigidActor* actor0, const PxTransform& localFrame0, PxRigidActor* actor1, const PxTransform& localFrame1);

protected:
	PxD6Motion::Enum _motion[PxD6Axis::eCOUNT];
//	PxJointLimit _linearLimit;
//	PxJointLimitPair _twistLimit;
//	PxJointLimitCone _swingLimit;
	//PxD6JointDrive _drive[PxD6Drive::eCOUNT];
	PxReal _projectionLinearTolerance;
	PxReal _projectionAngularTolerance;
};



}
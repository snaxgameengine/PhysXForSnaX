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
	

static const GUID PHYSXSPHERICALJOINT_GUID = { 0x8e040d27, 0xac2f, 0x4142, { 0xb3, 0xa8, 0x15, 0x7a, 0xc0, 0xba, 0xe0, 0x50 } };



class PHYSXCHIPS_API PhysXSphericalJoint : public PhysXJoint
{
	CHIPDESC_DECL;
public:
	PhysXSphericalJoint();
	virtual ~PhysXSphericalJoint();

	virtual bool CopyChip(Chip *chip) override;
	virtual bool LoadChip(DocumentLoader &loader) override;
	virtual bool SaveChip(DocumentSaver &saver) const override;

	virtual PxJoint *CreateJoint(PxPhysics& physics, PxRigidActor* actor0, const PxTransform& localFrame0, PxRigidActor* actor1, const PxTransform& localFrame1);

	virtual bool IsLimitEnabled() const;
	virtual void GetLimitParams(float &yLimitAngle, float &zLimitAngle, float &limitRestitution, float &limitSpring, float &limitDamping, float &limitContactDistance) const;
	virtual float GetProjectionLinearTolerance() const;

	virtual void SetLimitEnable(bool b, bool chipSettings = true);
	virtual void SetLimitParams(float yLimitAngle, float zLimitAngle, float limitRestitution, float limitStiffness, float limitDamping, float limitContactDistance, bool chipSettings = true);
	virtual void SetProjectionLinearTolerance(float l, bool chipSettings = true);

protected:
	bool _limitEnable;
	float _yLimitAngle;
	float _zLimitAngle;
	float _limitRestitution;
	float _limitStiffness;
	float _limitDamping; 
	float _limitContactDistance;
	float _projectionLinearTolerance;
};



}
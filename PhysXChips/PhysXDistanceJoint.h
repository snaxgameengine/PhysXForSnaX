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


static const GUID PHYSXDISTANCEJOINT_GUID = { 0x28946abe, 0x6d76, 0x4282, { 0xb3, 0x8d, 0x99, 0xd7, 0x63, 0x52, 0x65, 0xbf } };



class PHYSXCHIPS_API PhysXDistanceJoint : public PhysXJoint
{
	CHIPDESC_DECL;
public:
	PhysXDistanceJoint();
	virtual ~PhysXDistanceJoint();

	virtual bool CopyChip(Chip *chip) override;
	virtual bool LoadChip(DocumentLoader &loader) override;
	virtual bool SaveChip(DocumentSaver &saver) const override;

	virtual PxJoint *CreateJoint(PxPhysics& physics, PxRigidActor* actor0, const PxTransform& localFrame0, PxRigidActor* actor1, const PxTransform& localFrame1);

	virtual bool IsMinDistanceEnabled() const;
	virtual bool IsMaxDistanceEnabled() const;
	virtual float GetMinDistance() const;
	virtual float GetMaxDistance() const;
	virtual float GetTolerance() const;
	virtual bool IsSpringEnabled() const;
	virtual float GetStiffness() const;
	virtual float GetDamping() const;

	virtual void SetMinDistanceEnabled(bool b, bool chipSettings = true);
	virtual void SetMaxDistanceEnabled(bool b, bool chipSettings = true);
	virtual void SetDistanceRange(float minD, float maxD, bool chipSettings = true);
	virtual void SetTolerance(float d, bool chipSettings = true);
	virtual void SetSpringEnabled(bool b, bool chipSettings = true);
	virtual void SetStiffness(float d, bool chipSettings = true);
	virtual void SetDamping(float d, bool chipSettings = true);

protected:
	bool _minDistanceEnable;
	bool _maxDistanceEnable;
	float _minDistance;
	float _maxDistance;
	float _tolerance;
	bool _springEnable;
	float _stiffness;
	float _damping;

};



}
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
#include "M3DEngine/Chip.h"
#include "PhysXRigidBody.h"
#include "PhysX.h"


namespace m3d
{

	
static const GUID PHYSXRIGIDDYNAMIC_GUID = { 0x456d654d, 0xa3bf, 0x40da, { 0xae, 0x84, 0xcf, 0x20, 0xbf, 0x2a, 0xce, 0x40 } };



class PHYSXCHIPS_API PhysXRigidDynamic : public PhysXRigidBody
{
	CHIPDESC_DECL;
public:
	PhysXRigidDynamic();
	virtual ~PhysXRigidDynamic();

	virtual bool CopyChip(Chip *chip) override;
	virtual bool LoadChip(DocumentLoader &loader) override;
	virtual bool SaveChip(DocumentSaver &saver) const override;

	virtual void CallChip();

	virtual PxRigidDynamic *CreateActor(const PxTransform &pose);
	virtual void DestroyActor();

	virtual PxRigidDynamic *GetRigidDynamic() const { return _actor; }
	virtual PxRigidBody *GetRigidBody() const override { return GetRigidDynamic(); }

	virtual void AddForce(const PhysXForce &force);
	virtual void ClearForces();

	virtual bool IsKinematic();
	virtual float GetLinearDamping();
	virtual float GetAngularDamping();
	virtual float GetMaxAngularVelocity();
	virtual float GetSleepThreshold();
	virtual void GetMinSolverIterations(unsigned &pos, unsigned &vel);
	virtual float GetContactReportThreshold();

	virtual void SetKinematic(bool k, bool chipSettings = true);
	virtual void SetLinearDamping(float f, bool chipSettings = true);
	virtual void SetAngularDamping(float f, bool chipSettings = true);
	virtual void SetMaxAngularVelocity(float f, bool chipSettings = true);
	virtual void SetSleepThreshold(float f, bool chipSettings = true);
	virtual void SetMinSolverIterations(unsigned pos, unsigned vel, bool chipSettings = true);
	virtual void SetContactReportThreshold(float f, bool chipSettings = true);

protected:
	PxRigidDynamic *_actor;
	bool _kinematic;
	float _linearDamping;
	float _angularDamping;
	float _maxAngularVelocity;
	float _sleepThreshold;
	unsigned _minPositionIters;
	unsigned _minVelocityIters;
	float _contactReportThreshold;

};



}
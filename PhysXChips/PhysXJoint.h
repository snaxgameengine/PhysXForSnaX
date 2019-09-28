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
#include "PhysX.h"

namespace m3d
{

	
static const GUID PHYSXJOINT_GUID = { 0x9e56c487, 0x4d1e, 0x48d4, { 0xb8, 0xd9, 0x50, 0xf0, 0xff, 0xfd, 0x1b, 0x92 } };



class PHYSXCHIPS_API PhysXJoint : public Chip, public PhysXSceneObject
{
	CHIPDESC_DECL;
public:
	PhysXJoint();
	virtual ~PhysXJoint();

	virtual bool InitChip() override;
	virtual bool CopyChip(Chip *chip) override;
	virtual bool LoadChip(DocumentLoader &loader) override;
	virtual bool SaveChip(DocumentSaver &saver) const override;

	virtual void CallChip() override;

	virtual void OnSceneDestroyed() override { DestroyJoint(); }

	virtual bool Exist();
	virtual PxJoint *GetJoint() const { return _joint; }
	virtual void DestroyJoint();

	virtual PxJoint *CreateJoint(PxPhysics& physics, PxRigidActor* actor0, const PxTransform& localFrame0, PxRigidActor* actor1, const PxTransform& localFrame1) { return nullptr; }

	virtual bool IsGlobalFrame() const { return _globalFrame; }
	virtual void SetGlobalFrame(bool globalFrame);

	virtual bool IsBroken() const;
	virtual bool IsCollisionEnabled() const;
	virtual bool IsProjectionEnabled() const;
	virtual bool IsVisualization() const;
	virtual void GetBreakForce(float &f, float &t) const;
	virtual void SetCollisionEnabled(bool b, bool chipSettings = true);
	virtual void SetProjectionEnabled(bool b, bool chipSettings = true);
	virtual void SetVisualization(bool b, bool chipSettings = true);
	virtual void SetBreakForce(float f, float t, bool chipSettings = true);

protected:
	PxJoint *_joint;
	bool _globalFrame;
	bool _collisionEnabled;
	bool _projectionEnabled;
	bool _visualization;
	float _breakForce;
	float _breakTorque;

	virtual void UpdateConnections();
	virtual void OnRelease() { DestroyJoint(); }

};

}
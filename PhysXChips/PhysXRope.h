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


static const GUID PHYSXROPE_GUID = { 0x62ea0e5d, 0x90c8, 0x4898, { 0xae, 0x64, 0x14, 0xb0, 0x5a, 0x95, 0xbd, 0x58 } };



class PHYSXCHIPS_API PhysXRope : public Chip, public PhysXSceneObject
{
	CHIPDESC_DECL;
public:
	PhysXRope();
	virtual ~PhysXRope();

/*	virtual bool InitChip() override;
	virtual bool CopyChip(Chip *chip) override;
	virtual bool LoadChip(DocumentLoader &loader) override;
	virtual bool SaveChip(DocumentSaver &saver) const override;
*/
	virtual void CallChip() override;

	virtual void OnSceneDestroyed() override { DestroyRope(); }

	virtual void DestroyRope();

	virtual const List<PxRigidDynamic*> &GetActors() const { return _actors; }
	virtual const List<PxJoint*> &GetJoints() const { return _joints; }

protected:
	virtual void OnRelease() { DestroyRope(); }

	List<PxRigidDynamic*> _actors;
	List<PxJoint*> _joints;

};

}
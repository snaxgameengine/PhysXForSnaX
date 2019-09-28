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
#include "PhysXRigidDynamic.h"
#include "PhysX.h"


namespace m3d
{


static const GUID PHYSXROPEACTOR_GUID = { 0x739a253a, 0xb545, 0x41a5, { 0x95, 0x59, 0xf8, 0x50, 0xb3, 0x72, 0xd4, 0xb9 } };



class PHYSXCHIPS_API PhysXRopeActor : public PhysXRigidDynamic
{
	CHIPDESC_DECL;
public:
	PhysXRopeActor();
	virtual ~PhysXRopeActor();

	virtual bool CopyChip(Chip *chip) override;
	virtual bool LoadChip(DocumentLoader &loader) override;
	virtual bool SaveChip(DocumentSaver &saver) const override;

	virtual void CallChip() override {}

	virtual PxRigidDynamic *GetRigidDynamic() const override;

	virtual PxRigidDynamic *CreateActor(const PxTransform &pose) override { return nullptr; }
	virtual void DestroyActor() override {}

	enum Type {NONE, FIRST, LAST, INDEX };

	virtual Type GetType() const { return _type; }
	virtual void SetType(Type type);

protected:
	Type _type;
};



}
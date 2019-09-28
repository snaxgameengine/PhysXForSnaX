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


static const GUID PHYSXACTOR_GUID = { 0xd2cbb0af, 0x9319, 0x4a04, { 0x9d, 0x8f, 0xb5, 0xe1, 0x60, 0x39, 0x6d, 0x93 } };



class PHYSXCHIPS_API PhysXActor : public Chip, public PhysXSceneObject
{
	CHIPDESC_DECL;
public:
	PhysXActor();
	virtual ~PhysXActor();

	virtual bool CopyChip(Chip *chip) override;
	virtual bool LoadChip(DocumentLoader &loader) override;
	virtual bool SaveChip(DocumentSaver &saver) const override;

	virtual void OnSceneDestroyed() override { DestroyActor(); }

	virtual bool Exist() const;
	virtual PxActor *GetActor() const { return nullptr; }
	virtual void DestroyActor() {}

	virtual bool IsDisableGravity() const;
	virtual bool IsSendSleepNotifies() const;
	virtual bool IsVisualization() const;
	virtual PxDominanceGroup GetDominanceGroup() const; 

	virtual void SetDisableGravity(bool b, bool chipSettings = true);
	virtual void SetSendSleepNotifies(bool b, bool chipSettings = true);
	virtual void SetVisualization(bool b, bool chipSettings = true);
	virtual void SetDominanceGroup(PxDominanceGroup b, bool chipSettings = true);

protected:
	bool _disableGravity;
	bool _sendSleepNotifies;
	bool _visualization;
	PxDominanceGroup _dominanceGroup;

	virtual void OnRelease() { DestroyActor(); }

};



}
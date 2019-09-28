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


static const GUID PHYSXARTICULATIONBASE_GUID = { 0x6ae4523d, 0xc066, 0x46b9,{ 0x95, 0xbd, 0x3c, 0x83, 0xe2, 0x60, 0x1f, 0xfc } };



class PHYSXCHIPS_API PhysXArticulationBase : public Chip, public PhysXSceneObject
{
	CHIPDESC_DECL;
public:
	PhysXArticulationBase();
	~PhysXArticulationBase();

	bool CopyChip(Chip *chip) override;
	bool LoadChip(DocumentLoader &loader) override;
	bool SaveChip(DocumentSaver &saver) const override;

	void OnSceneDestroyed() override { DestroyArticulation(); }

	virtual bool Exist() const { return false; }
	virtual PxArticulationBase *GetArticulation() const { return nullptr; }
	virtual void DestroyArticulation() {}

	virtual float GetSleepThreshold();
	virtual void GetMinSolverIterations(unsigned &pos, unsigned &vel);

	virtual void SetSleepThreshold(float f);
	virtual void SetMinSolverIterations(unsigned pos, unsigned vel);

protected:
	float _sleepThreshold;
	unsigned _minPositionIters;
	unsigned _minVelocityIters;

	void OnRelease() override { DestroyArticulation(); }
	
};

}
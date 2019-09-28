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
#include "PhysXArticulationBase.h"
#include "PhysXScene.h"
#include "PhysXArticulationLink.h"

using namespace m3d;


CHIPDESCV1_DEF_HIDDEN(PhysXArticulationBase, L"PhysX Articulation Base", PHYSXARTICULATIONBASE_GUID, CHIP_GUID);



PhysXArticulationBase::PhysXArticulationBase()
{
	CREATE_CHILD(0, PHYSXSCENE_GUID, false, BOTH, L"Scene");
	CREATE_CHILD(1, PHYSXARTICULATIONLINK_GUID, false, BOTH, L"Root Link");

	_sleepThreshold = 0.05f;
	_minPositionIters = 4;
	_minVelocityIters = 1;
}

PhysXArticulationBase::~PhysXArticulationBase()
{
}

bool PhysXArticulationBase::CopyChip(Chip *chip)
{
	PhysXArticulationBase *c = dynamic_cast<PhysXArticulationBase*>(chip);
	B_RETURN(Chip::CopyChip(c));
	_sleepThreshold = c->_sleepThreshold;
	_minPositionIters = c->_minPositionIters;
	_minVelocityIters = c->_minVelocityIters;
	return true;
}

bool PhysXArticulationBase::LoadChip(DocumentLoader &loader)
{
	B_RETURN(Chip::LoadChip(loader));
	LOADDEF("sleepThreshold", _sleepThreshold, 0.05f);
	LOADDEF("minPositionIters", _minPositionIters, 4);
	LOADDEF("minVelocityIters", _minVelocityIters, 1);
	return true;
}

bool PhysXArticulationBase::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(Chip::SaveChip(saver));
	SAVEDEF("sleepThreshold", _sleepThreshold, 0.05f);
	SAVEDEF("minPositionIters", _minPositionIters, 4);
	SAVEDEF("minVelocityIters", _minVelocityIters, 1);
	return true;
}

float PhysXArticulationBase::GetSleepThreshold()
{
	return _sleepThreshold;
}

void PhysXArticulationBase::GetMinSolverIterations(unsigned &pos, unsigned &vel)
{
	pos = _minPositionIters;
	vel = _minVelocityIters;
}

void PhysXArticulationBase::SetSleepThreshold(float f)
{
	_sleepThreshold = f;

	PxArticulationBase *a = GetArticulation();
	if (a)
		a->setSleepThreshold(f);
}

void PhysXArticulationBase::SetMinSolverIterations(unsigned pos, unsigned vel)
{
	_minPositionIters = pos;
	_minVelocityIters = vel;

	PxArticulationBase *a = GetArticulation();
	if (a)
		a->setSolverIterationCounts(pos, vel);
}
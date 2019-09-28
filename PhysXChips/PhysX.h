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


namespace m3d
{

class PHYSXCHIPS_API PhysXSceneObject
{
public:
	virtual void OnSceneDestroyed() = 0;
};

struct PhysXForce
{
	enum Frame { GLOBAL_AT_GLOBAL_POS, GLOBAL_AT_LOCAL_POS, LOCAL_AT_GLOBAL_POS, LOCAL_AT_LOCAL_POS, GLOBAL_TORQUE, LOCAL_TORQUE };
	Frame frame;
	PxVec3 force;
	PxVec3 pos; // Not for torque!
	PxForceMode::Enum mode;

	PhysXForce(Frame frame, const PxVec3 &force, const PxVec3 &pos = PxVec3(0.0f, 0.0f, 0.0f), PxForceMode::Enum mode = PxForceMode::eFORCE) : frame(frame), force(force), pos(pos), mode(mode) {}
};

struct PhysXActorData
{
	PxActor *actor;
	PxTransform globalPose;
	List<PhysXForce> forces;
	PxTransform kinematicTarget;
	bool moveKinematic;
	Chip *chip;
	Chip *linkChip;

	PhysXActorData(PxActor *actor) : actor(actor), moveKinematic(false), chip(nullptr), linkChip(nullptr) {}
};



}
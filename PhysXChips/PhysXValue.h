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
#include "StdChips/Value.h"

namespace m3d
{


static const GUID PHYSXVALUE_GUID = { 0x388c1557, 0x310f, 0x4284, { 0xb2, 0x9b, 0x8f, 0x8e, 0xd1, 0xea, 0x95, 0xd3 } };



class PHYSXCHIPS_API PhysXValue : public Value
{
	CHIPDESC_DECL;
public:
	PhysXValue();
	virtual ~PhysXValue();

	virtual bool CopyChip(Chip *chip) override;
	virtual bool LoadChip(DocumentLoader &loader) override;
	virtual bool SaveChip(DocumentSaver &saver) const override;

	virtual value GetValue() override;
	virtual void SetValue(value v) override;

	enum Command { CMD_NONE, 
		CMD_SCENE_EXIST,
		CMD_SCENE_IS_RUNNING,
		CMD_SCENE_IS_SIMULATING,
		CMD_SCENE_REAL_TIME_INDEX,
		CMD_SCENE_SIM_INDEX,
		CMD_SCENE_SIM_WAIT,
		CMD_ACTOR_EXIST = 100,
		CMD_RIGID_BODY_XET_MASS = 150,
		CMD_RIGID_DYNAMIC_XET_LINEAR_DAMPING = 160,
		CMD_RIGID_DYNAMIC_XET_ANGULAR_DAMPING,
		CMD_RIGID_DYNAMIC_XET_MAX_ANGULAR_VELOCITY,
		CMD_RIGID_DYNAMIC_XET_SLEEP_THRESHOLD,
		CMD_RIGID_DYNAMIC_IS_SLEEPING,
		CMD_JOINT_EXIST = 200
	};

	virtual Command GetCommand() const { return _cmd; }
	virtual void SetCommand(Command cmd);

protected:
	Command _cmd;

};

}
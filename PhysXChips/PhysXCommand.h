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

namespace m3d
{

static const GUID PHYSXCOMMAND_GUID = { 0x8348dec5, 0x8ea4, 0x481a, { 0x9e, 0x41, 0x32, 0xf8, 0x14, 0xdf, 0xee, 0x29 } };



class PHYSXCHIPS_API PhysXCommand : public Chip
{
	CHIPDESC_DECL;
public:
	PhysXCommand();
	virtual ~PhysXCommand();

	virtual bool CopyChip(Chip *chip) override;
	virtual bool LoadChip(DocumentLoader &loader) override;
	virtual bool SaveChip(DocumentSaver &saver) const override;

	virtual void CallChip();

	enum Command { CMD_NONE, 
		CMD_SCENE_SIMULATE, 
		CMD_SCENE_FETCH_RESULTS, 
		CMD_SCENE_DEBUG_RENDER,
		CMD_SCENE_START,
		CMD_SCENE_STOP,
		CMD_SCENE_DESTROY,
		CMD_ACTOR_DESTROY = 400,
		CMD_RIGIDDYNAMIC_ADD_GLOBAL_FORCE_AT_GLOBAL_POINT = 500,
		CMD_RIGIDDYNAMIC_ADD_GLOBAL_FORCE_AT_LOCAL_POINT,
		CMD_RIGIDDYNAMIC_ADD_LOCAL_FORCE_AT_GLOBAL_POINT,
		CMD_RIGIDDYNAMIC_ADD_LOCAL_FORCE_AT_LOCAL_POINT,
		CMD_RIGIDDYNAMIC_ADD_GLOBAL_TORQUE,
		CMD_RIGIDDYNAMIC_ADD_LOCAL_TORQUE,
		CMD_RIGIDDYNAMIC_CLEAR_FORCES_AND_TORQUES,
		CMD_JOINT_DESTROY = 600
	};

	virtual Command GetCommand() const { return _cmd; }
	virtual void SetCommand(Command cmd);

protected:
	Command _cmd;
};



}
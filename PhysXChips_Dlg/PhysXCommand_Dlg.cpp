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
#include "PhysXCommand_Dlg.h"

using namespace m3d;


DIALOGDESC_DEF(PhysXCommand_Dlg, PHYSXCOMMAND_GUID);


void PhysXCommand_Dlg::Init()
{
	RPtr sysop = AddItem(L"Scene Commands", PhysXCommand::CMD_NONE);
	AddItem(sysop, L"Simulate", PhysXCommand::CMD_SCENE_SIMULATE);
	AddItem(sysop, L"Fetch Results", PhysXCommand::CMD_SCENE_FETCH_RESULTS);
	AddItem(sysop, L"Render Debug Geometry", PhysXCommand::CMD_SCENE_DEBUG_RENDER);
	AddItem(sysop, L"Start Simulation", PhysXCommand::CMD_SCENE_START);
	AddItem(sysop, L"Stop Simulation", PhysXCommand::CMD_SCENE_STOP);
	AddItem(sysop, L"Destroy Scene", PhysXCommand::CMD_SCENE_DESTROY);

	RPtr actorop = AddItem(L"Actor Commands", PhysXCommand::CMD_NONE);
	AddItem(actorop, L"Destroy Actor", PhysXCommand::CMD_ACTOR_DESTROY);

	RPtr dynamicop = AddItem(L"Rigid Dynamic Commands", PhysXCommand::CMD_NONE);
	AddItem(dynamicop, L"Add Global Force at Global Position", PhysXCommand::CMD_RIGIDDYNAMIC_ADD_GLOBAL_FORCE_AT_GLOBAL_POINT);
	AddItem(dynamicop, L"Add Global Force at Local Position", PhysXCommand::CMD_RIGIDDYNAMIC_ADD_GLOBAL_FORCE_AT_LOCAL_POINT);
	AddItem(dynamicop, L"Add Local Force at Global Position", PhysXCommand::CMD_RIGIDDYNAMIC_ADD_LOCAL_FORCE_AT_GLOBAL_POINT);
	AddItem(dynamicop, L"Add Local Force at Local Position", PhysXCommand::CMD_RIGIDDYNAMIC_ADD_LOCAL_FORCE_AT_LOCAL_POINT);
	AddItem(dynamicop, L"Add Global Torque", PhysXCommand::CMD_RIGIDDYNAMIC_ADD_GLOBAL_TORQUE);
	AddItem(dynamicop, L"Add Local Torque", PhysXCommand::CMD_RIGIDDYNAMIC_ADD_LOCAL_TORQUE);
	AddItem(dynamicop, L"Clear Forces and Torques", PhysXCommand::CMD_RIGIDDYNAMIC_CLEAR_FORCES_AND_TORQUES);

	RPtr jointop = AddItem(L"Joint Commands", PhysXCommand::CMD_NONE);
	AddItem(jointop, L"Destroy", PhysXCommand::CMD_JOINT_DESTROY);

	sort();

	SetSelectionChangedCallback([this](RData data) -> bool {
		PhysXCommand::Command cmd = (PhysXCommand::Command)data;
		if (cmd == GetChip()->GetCommand())
			return false;
		GetChip()->SetCommand(cmd);
		return true;
		});

	SetInit(GetChip()->GetCommand(), PhysXCommand::CMD_NONE);
}

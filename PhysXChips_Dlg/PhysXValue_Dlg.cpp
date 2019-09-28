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
#include "PhysXValue_Dlg.h"

using namespace m3d;


DIALOGDESC_DEF(PhysXValue_Dlg, PHYSXVALUE_GUID);



void PhysXValue_Dlg::Init()
{
	RPtr sceneop = AddItem(L"Scene Values", PhysXValue::CMD_NONE);
	AddItem(sceneop, L"Exist?", PhysXValue::CMD_SCENE_EXIST);
	AddItem(sceneop, L"Is Running?", PhysXValue::CMD_SCENE_IS_RUNNING);
	AddItem(sceneop, L"Is Simulating?", PhysXValue::CMD_SCENE_IS_SIMULATING);
	AddItem(sceneop, L"Real-Time Index", PhysXValue::CMD_SCENE_REAL_TIME_INDEX);
	AddItem(sceneop, L"Simulation Index", PhysXValue::CMD_SCENE_SIM_INDEX);
	AddItem(sceneop, L"Simulation Delay (ms)", PhysXValue::CMD_SCENE_SIM_WAIT);

	RPtr actorop = AddItem(L"Actor Values", PhysXValue::CMD_NONE);
	AddItem(actorop, L"Exist?", PhysXValue::CMD_ACTOR_EXIST);

	RPtr rigidbodyop = AddItem(L"Rigid Body Values", PhysXValue::CMD_NONE);
	AddItem(rigidbodyop, L"Get/Set Mass", PhysXValue::CMD_RIGID_BODY_XET_MASS);

	RPtr rigiddynamicop = AddItem(L"Rigid Dynamic Values", PhysXValue::CMD_NONE);
	AddItem(rigiddynamicop, L"Get/Set Linear Damping", PhysXValue::CMD_RIGID_DYNAMIC_XET_LINEAR_DAMPING);
	AddItem(rigiddynamicop, L"Get/Set Angular Damping", PhysXValue::CMD_RIGID_DYNAMIC_XET_ANGULAR_DAMPING);
	AddItem(rigiddynamicop, L"Get/Set Max Angular Velocity", PhysXValue::CMD_RIGID_DYNAMIC_XET_MAX_ANGULAR_VELOCITY);
	AddItem(rigiddynamicop, L"Get/Set Sleep Threshold", PhysXValue::CMD_RIGID_DYNAMIC_XET_SLEEP_THRESHOLD);
	AddItem(rigiddynamicop, L"Is Sleeping?", PhysXValue::CMD_RIGID_DYNAMIC_IS_SLEEPING);

	RPtr jointop = AddItem(L"Joint Values", PhysXValue::CMD_NONE);
	AddItem(jointop, L"Exist?", PhysXValue::CMD_JOINT_EXIST);

	sort();

	SetSelectionChangedCallback([this](RData data) -> bool {
		PhysXValue::Command cmd = (PhysXValue::Command)data;
		if (cmd == GetChip()->GetCommand())
			return false;
		GetChip()->SetCommand(cmd);
		return true;
		});

	SetInit(GetChip()->GetCommand(), PhysXValue::CMD_NONE);
}

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
#include "PhysXVector_Dlg.h"

using namespace m3d;


DIALOGDESC_DEF(PhysXVector_Dlg, PHYSXVECTOR_GUID);


void PhysXVector_Dlg::Init()
{
	RPtr sceneop = AddItem(L"Scene Vectors", PhysXVector::CMD_NONE);
	AddItem(sceneop, L"Get/Set Gravity", PhysXVector::CMD_SCENE_XET_GRAVITY);

	RPtr rigidbodyop = AddItem(L"Rigid Body Vectors", PhysXVector::CMD_NONE);
	AddItem(rigidbodyop, L"Get/Set Angular Velocity", PhysXVector::CMD_RIGID_BODY_XET_ANGULAR_VELOCITY);
	AddItem(rigidbodyop, L"Get/Set Linear Velocity", PhysXVector::CMD_RIGID_BODY_XET_LINEAR_VELOCITY);
	AddItem(rigidbodyop, L"Get/Set Mass Space Inertia Tensor", PhysXVector::CMD_RIGID_BODY_XET_MASS_SPACE_INERTIA_TENSOR);

	RPtr jointop = AddItem(L"Joint Vectors", PhysXVector::CMD_NONE);
	AddItem(jointop, L"Get Linear Force", PhysXVector::CMD_CONSTRAINT_GET_LINEAR_FORCE);
	AddItem(jointop, L"Get Angular Force", PhysXVector::CMD_CONSTRAINT_GET_ANGULAR_FORCE);

	sort();

	SetSelectionChangedCallback([this](RData data) -> bool {
		PhysXVector::Command cmd = (PhysXVector::Command)data;
		if (cmd == GetChip()->GetCommand())
			return false;
		GetChip()->SetCommand(cmd);
		return true;
		});

	SetInit(GetChip()->GetCommand(), PhysXVector::CMD_NONE);
}

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
#include "PhysXJoint_Dlg.h"

using namespace m3d;


DIALOGDESC_DEF(PhysXJoint_Dlg, PHYSXJOINT_GUID);


void PhysXJoint_Dlg::Init()
{
	AddPushButton(L"Destroy Joint", [this](Id id, RVariant v) { GetChip()->DestroyJoint(); });
	AddCheckBox(L"Global Frame", GetChip()->IsGlobalFrame() ? RCheckState::Checked : RCheckState::Unchecked, [this](Id id, RVariant v) { GetChip()->SetGlobalFrame(v.ToUInt() == RCheckState::Checked); });
	AddCheckBox(L"Collision", GetChip()->IsCollisionEnabled() ? RCheckState::Checked : RCheckState::Unchecked, [this](Id id, RVariant v) { GetChip()->SetCollisionEnabled(v.ToUInt() == RCheckState::Checked); });
	AddCheckBox(L"Projection", GetChip()->IsProjectionEnabled() ? RCheckState::Checked : RCheckState::Unchecked, [this](Id id, RVariant v) { GetChip()->SetVisualization(v.ToUInt() == RCheckState::Checked); });

	float f, t;
	GetChip()->GetBreakForce(f, t);
	AddDoubleSpinBox(1, L"Break Force", f, 0, FLT_MAX, 1, [this](Id id, RVariant v) { GetChip()->SetBreakForce(v.ToFloat(), GetValueFromWidget(2).ToFloat()); });
	AddDoubleSpinBox(2, L"Break Torque", t, 0, FLT_MAX, 1, [this](Id id, RVariant v) { GetChip()->SetBreakForce(GetValueFromWidget(1).ToFloat(), v.ToFloat()); });
}

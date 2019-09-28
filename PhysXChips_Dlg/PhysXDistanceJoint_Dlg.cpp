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
#include "PhysXDistanceJoint_Dlg.h"

using namespace m3d;


DIALOGDESC_DEF(PhysXDistanceJoint_Dlg, PHYSXDISTANCEJOINT_GUID);


void PhysXDistanceJoint_Dlg::Init()
{
	AddCheckBox(1, L"Enable Min Limit", GetChip()->IsMinDistanceEnabled() ? RCheckState::Checked : RCheckState::Unchecked, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetMinDistanceEnabled(v.ToUInt() == RCheckState::Checked); _enableButtons(); });
	AddCheckBox(2, L"Enable Max Limit", GetChip()->IsMaxDistanceEnabled() ? RCheckState::Checked : RCheckState::Unchecked, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetMaxDistanceEnabled(v.ToUInt() == RCheckState::Checked); _enableButtons(); });
	AddDoubleSpinBox(3, L"Min Distance", GetChip()->GetMinDistance(), 0.0f, FLT_MAX, 0.1f, [this](Id id, RVariant v) { GetChip()->SetDistanceRange(v.ToFloat(), GetChip()->GetMaxDistance()); });
	AddDoubleSpinBox(4, L"Max Distance", GetChip()->GetMaxDistance(), 0.0f, FLT_MAX, 0.1f, [this](Id id, RVariant v) { GetChip()->SetDistanceRange(GetChip()->GetMinDistance(), v.ToFloat()); });
	AddDoubleSpinBox(5, L"Limit Restitution", GetChip()->GetTolerance(), 0.0f, FLT_MAX, 0.1f, [this](Id id, RVariant v) { GetChip()->SetTolerance(v.ToFloat()); });
	AddCheckBox(6, L"Limit Spring", GetChip()->IsSpringEnabled() ? RCheckState::Checked : RCheckState::Unchecked, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetSpringEnabled(v.ToUInt() == RCheckState::Checked); _enableButtons(); });
	AddDoubleSpinBox(7, L"Limit Damping", GetChip()->GetStiffness(), 0.0f, FLT_MAX, 0.1f, [this](Id id, RVariant v) { GetChip()->SetStiffness(v.ToFloat()); });
	AddDoubleSpinBox(8, L"Limit Contact Distance", GetChip()->GetDamping(), 0.0f, FLT_MAX, 0.1f, [this](Id id, RVariant v) { GetChip()->SetDamping(v.ToFloat()); });

	_enableButtons();
}

void PhysXDistanceJoint_Dlg::_enableButtons()
{
	bool b = GetValueFromWidget(6).ToUInt() == RCheckState::Checked;
	SetWidgetEnabled(7, b);
	SetWidgetEnabled(8, b);
	bool b1 = GetValueFromWidget(1).ToUInt() == RCheckState::Checked;
	bool b2 = GetValueFromWidget(2).ToUInt() == RCheckState::Checked;
	SetWidgetEnabled(3, b1);
	SetWidgetEnabled(4, b2);
	SetWidgetEnabled(5, b1 || b2);
}
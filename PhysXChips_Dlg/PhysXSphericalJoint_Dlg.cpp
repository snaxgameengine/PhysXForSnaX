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
#include "PhysXSphericalJoint_Dlg.h"

using namespace m3d;


DIALOGDESC_DEF(PhysXSphericalJoint_Dlg, PHYSXSPHERICALJOINT_GUID);


void PhysXSphericalJoint_Dlg::Init()
{
	AddCheckBox(1, L"Enable Limits", GetChip()->IsLimitEnabled() ? RCheckState::Checked : RCheckState::Unchecked, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetLimitEnable(v.ToUInt() == RCheckState::Checked); _enableButtons(); });

	float y, z, r, s, d, c;
	GetChip()->GetLimitParams(y, z, r, s, d, c);

	auto F = [this](Id id, RVariant v)
	{
		SetDirty();
		float y, z, r, s, d, c;
		y = GetValueFromWidget(2).ToFloat();
		z = GetValueFromWidget(3).ToFloat();
		r = GetValueFromWidget(4).ToFloat();
		s = GetValueFromWidget(5).ToFloat();
		d = GetValueFromWidget(6).ToFloat();
		c = GetValueFromWidget(7).ToFloat();
		GetChip()->SetLimitParams(y, z, r, s, d, c);

	};

	AddDoubleSpinBox(2, L"y-Limit Angle", y, 0.01f, XM_PI - 0.01f, 0.1f, F);
	AddDoubleSpinBox(3, L"z-Limit Angle", z, 0.01f, XM_PI - 0.01f, 0.1f, F);
	AddDoubleSpinBox(4, L"Limit Restitution", r, 0.0f, 1.0f, 0.1f, F);
	AddDoubleSpinBox(5, L"Limit Spring", s, 0.0f, FLT_MAX, 0.1f, F);
	AddDoubleSpinBox(6, L"Limit Damping", d, 0.0f, FLT_MAX, 0.1f, F);
	AddDoubleSpinBox(7, L"Limit Contact Distance", c, 0.0f, FLT_MAX, 0.1f, F);
	AddDoubleSpinBox(8, L"Projection Linear Tolerance", GetChip()->GetProjectionLinearTolerance(), 0.0f, FLT_MAX, 0.1f, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetProjectionLinearTolerance(v.ToFloat()); });
	_enableButtons();
}

void PhysXSphericalJoint_Dlg::_enableButtons()
{
	bool b = GetValueFromWidget(1).ToUInt() == RCheckState::Checked;
	SetWidgetEnabled(2, b);
	SetWidgetEnabled(3, b);
	SetWidgetEnabled(4, b);
	SetWidgetEnabled(5, b);
	SetWidgetEnabled(6, b);
	SetWidgetEnabled(7, b);
}
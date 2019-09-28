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
#include "PhysXRigidDynamic_Dlg.h"

using namespace m3d;


DIALOGDESC_DEF(PhysXRigidDynamic_Dlg, PHYSXRIGIDDYNAMIC_GUID);



void PhysXRigidDynamic_Dlg::Init()
{
	AddCheckBox(L"Kinematic", GetChip()->IsKinematic() ? RCheckState::Checked : RCheckState::Unchecked, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetKinematic(v.ToUInt() == RCheckState::Checked); });
	AddDoubleSpinBox(L"Linear Damping", GetChip()->GetLinearDamping(), 0, FLT_MAX, 0.1, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetLinearDamping(v.ToFloat()); });
	AddDoubleSpinBox(L"Angular Damping", GetChip()->GetAngularDamping(), 0, FLT_MAX, 0.1, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetAngularDamping(v.ToFloat()); });
	AddDoubleSpinBox(L"Max Angular Velocity", GetChip()->GetMaxAngularVelocity(), 0.0001, FLT_MAX, 0.1, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetMaxAngularVelocity(v.ToFloat()); });
	AddDoubleSpinBox(L"Sleep Threshold", GetChip()->GetSleepThreshold(), 0.0001, FLT_MAX, 0.1, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetSleepThreshold(v.ToFloat()); });

	unsigned p, v;
	GetChip()->GetMinSolverIterations(p, v);
	AddSpinBox(1, L"Minimum Position Iterations", p, 1, 255, 1, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetMinSolverIterations(v.ToInt(), GetValueFromWidget(2).ToInt()); });
	AddSpinBox(2, L"Minimum Velocity Iterations", v, 1, 255, 1, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetMinSolverIterations(GetValueFromWidget(1).ToInt(), v.ToInt()); });
	AddDoubleSpinBox(L"Contact Report Threshold", GetChip()->GetContactReportThreshold(), 0.0001, FLT_MAX, 0.1, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetContactReportThreshold(v.ToFloat()); });
}

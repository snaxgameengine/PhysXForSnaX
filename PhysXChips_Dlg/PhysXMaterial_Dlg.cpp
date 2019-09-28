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
#include "PhysXMaterial_Dlg.h"

using namespace m3d;


DIALOGDESC_DEF(PhysXMaterial_Dlg, PHYSXMATERIAL_GUID);


void PhysXMaterial_Dlg::Init()
{
	AddDoubleSpinBox(L"Static Friction", GetChip()->GetStaticFriction(), 0, FLT_MAX, 0.1f, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetStaticFriction(v.ToFloat()); });
	AddDoubleSpinBox(L"Dynamic Friction", GetChip()->GetDynamicFriction(), 0, FLT_MAX, 0.1f, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetDynamicFriction(v.ToFloat()); });
	AddDoubleSpinBox(L"Restitution", GetChip()->GetRestitution(), 0, 1, 0.1f, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetRestitution(v.ToFloat()); });
	AddCheckBox(1, L"Disable Friction", GetChip()->IsDisableFriction() ? RCheckState::Checked : RCheckState::Unchecked, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetDisableFriction(v.ToUInt() == RCheckState::Checked); _enableButtons(); });
	AddCheckBox(2, L"Disable Strong Friction", GetChip()->IsDisableStrongFriction() ? RCheckState::Checked : RCheckState::Unchecked, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetDisableStrongFriction(v.ToUInt() == RCheckState::Checked); });
	ComboBoxInitList lst;
	lst.push_back(std::make_pair(String(L"Average"), RVariant(physx::PxCombineMode::eAVERAGE)));
	lst.push_back(std::make_pair(String(L"Minimum"), RVariant(physx::PxCombineMode::eMIN)));
	lst.push_back(std::make_pair(String(L"Multiply"), RVariant(physx::PxCombineMode::eMULTIPLY)));
	lst.push_back(std::make_pair(String(L"Maximum"), RVariant(physx::PxCombineMode::eMAX)));
	AddComboBox(3, L"Friction Combine Mode", lst, GetChip()->GetFrictionCombineMode(), [this](Id id, RVariant v) { SetDirty(); GetChip()->SetFrictionCombineMode((physx::PxCombineMode::Enum)v.ToUInt()); });
	AddComboBox(4, L"Restitution Combine Mode", lst, GetChip()->GetRestitutionCombineMode(), [this](Id id, RVariant v) { SetDirty(); GetChip()->SetRestitutionCombineMode((physx::PxCombineMode::Enum)v.ToUInt()); });

	_enableButtons();
}

void PhysXMaterial_Dlg::_enableButtons()
{
	bool b = GetValueFromWidget(1).ToUInt() == RCheckState::Unchecked;
	SetWidgetEnabled(2, b);
	SetWidgetEnabled(3, b);
}
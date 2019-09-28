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
#include "PhysXScene_Dlg.h"

using namespace m3d;


DIALOGDESC_DEF(PhysXScene_Dlg, PHYSXSCENE_GUID);


void PhysXScene_Dlg::Init()
{
	ComboBoxInitList rate, simLimit, solverType, bpType, frictionType;
	rate.push_back(std::make_pair(String(L"30 Hz"), 30.0));
	rate.push_back(std::make_pair(String(L"50 Hz"), 50.0));
	rate.push_back(std::make_pair(String(L"60 Hz"), 60.0));
	rate.push_back(std::make_pair(String(L"75 Hz"), 75.0));
	rate.push_back(std::make_pair(String(L"90 Hz"), 90.0));
	rate.push_back(std::make_pair(String(L"100 Hz"), 100.0));
	rate.push_back(std::make_pair(String(L"120 Hz"), 120.0));
	rate.push_back(std::make_pair(String(L"140 Hz"), 140.0));
	rate.push_back(std::make_pair(String(L"180 Hz"), 180.0));
	rate.push_back(std::make_pair(String(L"200 Hz"), 200.0));
	rate.push_back(std::make_pair(String(L"250 Hz"), 250.0));
	rate.push_back(std::make_pair(String(L"500 Hz"), 500.0));

	simLimit.push_back(std::make_pair(String(L"10 ms"), 10.0));
	simLimit.push_back(std::make_pair(String(L"20 ms"), 20.0));
	simLimit.push_back(std::make_pair(String(L"50 ms"), 50.0));
	simLimit.push_back(std::make_pair(String(L"100 ms"), 100.0));
	simLimit.push_back(std::make_pair(String(L"200 ms"), 200.0));

	bpType.push_back(std::make_pair(String(L"SAP (3-axes sweep-and-prune)"), (unsigned)PxBroadPhaseType::eSAP));
	bpType.push_back(std::make_pair(String(L"MBP (Multi box pruning)"), (unsigned)PxBroadPhaseType::eMBP));
	bpType.push_back(std::make_pair(String(L"ABP (Automatic box pruning)"), (unsigned)PxBroadPhaseType::eABP));
	bpType.push_back(std::make_pair(String(L"GPU (GPU accelerated)"), (unsigned)PxBroadPhaseType::eGPU));

	solverType.push_back(std::make_pair(String(L"PGS (Projected Gauss-Seidel iterative solver)"), (unsigned)PxSolverType::ePGS));
	solverType.push_back(std::make_pair(String(L"TGS (Temporal Gauss-Seidel solver)"), (unsigned)PxSolverType::eTGS));

	frictionType.push_back(std::make_pair(String(L"Patch Friction Model"), (unsigned)PxFrictionType::ePATCH));
	frictionType.push_back(std::make_pair(String(L"One Directional Per-Contact Friction Model"), (unsigned)PxFrictionType::eONE_DIRECTIONAL));
	frictionType.push_back(std::make_pair(String(L"Two Directional Per-Contact Friction Model"), (unsigned)PxFrictionType::eTWO_DIRECTIONAL));

	AddComboBox(L"Simulation Rate:", rate, GetChip()->GetSimulationRate(), [this](Id id, RVariant v) { SetDirty(); GetChip()->SetSimulationRate(v.ToDouble()); });
	AddComboBox(L"Maximum Simulation Time:", simLimit, GetChip()->GetMaxSimulationTime(), [this](Id id, RVariant v) { SetDirty(); GetChip()->SetMaxSimulationTime(v.ToDouble()); });
	AddComboBox(L"Solver Type:", solverType, (unsigned)GetChip()->GetSolverType(), [this](Id id, RVariant v) { SetDirty(); GetChip()->SetSolverType((physx::PxSolverType::Enum)v.ToUInt()); });
	AddComboBox(L"Broad Phase Type:", bpType, (unsigned)GetChip()->GetBroadPhaseAlgorithm(), [this](Id id, RVariant v) { SetDirty(); GetChip()->SetBroadPhaseAlgorithm((physx::PxBroadPhaseType::Enum)v.ToUInt()); });
	AddComboBox(L"Friction Type:", frictionType, (unsigned)GetChip()->GetFrictionType(), [this](Id id, RVariant v) { SetDirty(); GetChip()->SetFrictionType((physx::PxFrictionType::Enum)v.ToUInt()); });
	
	auto F = [this](Id id, RVariant v)
	{
		SetDirty();
		PxSceneFlags flags;
		if (GetValueFromWidget(1).ToUInt() == RCheckState::Checked) flags |= PxSceneFlag::eENABLE_CCD;
		if (GetValueFromWidget(2).ToUInt() == RCheckState::Unchecked) flags |= PxSceneFlag::eDISABLE_CCD_RESWEEP;
		if (GetValueFromWidget(3).ToUInt() == RCheckState::Checked)  flags |= PxSceneFlag::eENABLE_ENHANCED_DETERMINISM;
		if (GetValueFromWidget(4).ToUInt() == RCheckState::Checked) flags |= PxSceneFlag::eENABLE_FRICTION_EVERY_ITERATION;
		if (GetValueFromWidget(5).ToUInt() == RCheckState::Checked) flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
		if (GetValueFromWidget(6).ToUInt() == RCheckState::Checked) flags |= PxSceneFlag::eENABLE_PCM;
		if (GetValueFromWidget(7).ToUInt() == RCheckState::Checked) flags |= PxSceneFlag::eENABLE_STABILIZATION;
		if (GetValueFromWidget(8).ToUInt() == RCheckState::Checked) flags |= PxSceneFlag::eADAPTIVE_FORCE;
		if (GetValueFromWidget(9).ToUInt() == RCheckState::Checked) flags |= PxSceneFlag::eENABLE_AVERAGE_POINT;
		GetChip()->SetSceneFlags(flags);
	};

	unsigned flags = GetChip()->GetSceneFlags();
	AddCheckBox(1, L"Enable Continuous Collision Detection (CCD)", (flags & PxSceneFlag::eENABLE_CCD) != 0 ? RCheckState::Checked : RCheckState::Unchecked, F);
	AddCheckBox(2, L"Enable CCD Resweep", (flags & PxSceneFlag::eDISABLE_CCD_RESWEEP) == 0 ? RCheckState::Checked : RCheckState::Unchecked, F);
	AddCheckBox(3, L"Enable Enhanched Determinism", (flags & PxSceneFlag::eENABLE_ENHANCED_DETERMINISM) != 0 ? RCheckState::Checked : RCheckState::Unchecked, F);
	AddCheckBox(4, L"Enable Friction Every Iteration", (flags & PxSceneFlag::eENABLE_FRICTION_EVERY_ITERATION) != 0 ? RCheckState::Checked : RCheckState::Unchecked, F);
	AddCheckBox(5, L"Enable GPU Dynamics", (flags & PxSceneFlag::eENABLE_GPU_DYNAMICS) != 0 ? RCheckState::Checked : RCheckState::Unchecked, F);
	AddCheckBox(6, L"Enable PCM", (flags & PxSceneFlag::eENABLE_PCM) != 0 ? RCheckState::Checked : RCheckState::Unchecked, F);
	AddCheckBox(7, L"Enable Stabilization", (flags & PxSceneFlag::eENABLE_STABILIZATION) != 0 ? RCheckState::Checked : RCheckState::Unchecked, F);
	AddCheckBox(8, L"Enable Adaptive Forces", (flags & PxSceneFlag::eADAPTIVE_FORCE) != 0 ? RCheckState::Checked : RCheckState::Unchecked, F);
	AddCheckBox(9, L"Enable Average Points", (flags & PxSceneFlag::eENABLE_AVERAGE_POINT) != 0 ? RCheckState::Checked : RCheckState::Unchecked, F);
//	AddCheckBox(10, L"Enable Debug Visualization", RCheckState::Checked, [this](Id id, RVariant v) {});

	AddLine();
	AddPushButton(L"Start", [this](Id id, RVariant v) { GetChip()->GetScene(); GetChip()->StartSimulation(); });
	AddPushButton(L"Stop", [this](Id id, RVariant v) { GetChip()->StopSimulation(); });
	AddPushButton(L"Destroy", [this](Id id, RVariant v) { GetChip()->DestroyScene(); });
}


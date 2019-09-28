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
#include "PhysXArticulationJointRC_Dlg.h"

using namespace m3d;


DIALOGDESC_DEF(PhysXArticulationJointRC_Dlg, PHYSXARTICULATIONJOINTRC_GUID);



void PhysXArticulationJointRC_Dlg::Init()
{
	ComboBoxInitList type, axis, motion;
	type.push_back(std::make_pair(String(L"Prismatic"), PxArticulationJointType::ePRISMATIC));
	type.push_back(std::make_pair(String(L"Revolute"), PxArticulationJointType::eREVOLUTE));
	type.push_back(std::make_pair(String(L"Spherical"), PxArticulationJointType::eSPHERICAL));
	type.push_back(std::make_pair(String(L"Fixed"), PxArticulationJointType::eFIX));
	axis.push_back(std::make_pair(String(L"Twist"), PxArticulationAxis::eTWIST));
	axis.push_back(std::make_pair(String(L"Swing 1"), PxArticulationAxis::eSWING1));
	axis.push_back(std::make_pair(String(L"Swing 2"), PxArticulationAxis::eSWING2));
	axis.push_back(std::make_pair(String(L"X"), PxArticulationAxis::eX));
	axis.push_back(std::make_pair(String(L"Y"), PxArticulationAxis::eY));
	axis.push_back(std::make_pair(String(L"Z"), PxArticulationAxis::eZ));
	motion.push_back(std::make_pair(String(L"Locked"), PxArticulationMotion::eLOCKED));
	motion.push_back(std::make_pair(String(L"Limited"), PxArticulationMotion::eLIMITED));
	motion.push_back(std::make_pair(String(L"Free"), PxArticulationMotion::eFREE));

	auto *c = GetChip();

	AddCheckBox(1, L"Global Frame", c->IsGlobalFrame() ? RCheckState::Checked : RCheckState::Unchecked, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetGlobalFrame(v.ToUInt() == RCheckState::Checked); } );
	AddComboBox(2, L"Type", type, (unsigned)c->GetType(), [this](Id id, RVariant v) { SetDirty(); GetChip()->SetType((PxArticulationJointType::Enum)v.ToUInt()); } );
	AddDoubleSpinBox(3, L"Friction Coefficient", c->GetFrictionCoefficient(), 0.0f, FLT_MAX, 1.0f, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetFrictionCoefficient((PxReal)v.ToDouble()); } );
	AddDoubleSpinBox(4, L"Max Joint Velocity", c->GetMaxJointVelocity(), 0.0f, FLT_MAX, 1.0f, [this](Id id, RVariant v) { SetDirty(); GetChip()->SetMaxJointVelocity((PxReal)v.ToDouble()); } );
	AddLine();

	AddComboBox(5, L"Axis", axis, PxArticulationAxis::eTWIST, [this](Id id, RVariant v) {
		auto axis = (PxArticulationAxis::Enum)v.ToUInt();
		Activate(10 + axis); // Motion
		Activate(20 + axis); // Low Limit
		Activate(30 + axis); // High Limit
		Activate(40 + axis); // Stiffness
		Activate(50 + axis); // Damping
		Activate(60 + axis); // Max Force
		Activate(70 + axis); // Is Acceleration Drive
		Activate(80 + axis); // Drive Target
		Activate(90 + axis); // Drive Target Vel.
	} );

	const PhysXArticulationJointRC::AxisParams ap[PxArticulationAxis::eCOUNT] = { 
		c->GetAxisParams(PxArticulationAxis::eTWIST), c->GetAxisParams(PxArticulationAxis::eSWING1), c->GetAxisParams(PxArticulationAxis::eSWING2), 
		c->GetAxisParams(PxArticulationAxis::eX), c->GetAxisParams(PxArticulationAxis::eY), c->GetAxisParams(PxArticulationAxis::eZ) };

	// Motion
	{
		auto cb = [this](Id id, RVariant v) {
			auto axis = (PxArticulationAxis::Enum)(id - 10);
			GetChip()->SetMotion(axis, (PxArticulationMotion::Enum)v.ToUInt());
			SetDirty();
		};
		auto *w = AddComboBox(10, L"Motion", motion, ap[PxArticulationAxis::eTWIST].motion, cb);
		for (int i = 1; i < PxArticulationAxis::eCOUNT; i++)
			OverloadComboBox(10 + i, w, ap[PxArticulationAxis::eTWIST + i].motion, cb);
	}

	// Low Limit
	{
		auto cb = [this](Id id, RVariant v) {
			auto axis = (PxArticulationAxis::Enum)(id - 20);
			GetChip()->SetLowLimit(axis, (PxReal)v.ToDouble());
			SetDirty();
		};
		auto *w = AddDoubleSpinBox(20, L"Low Limit", ap[PxArticulationAxis::eTWIST].lowLimit, -FLT_MAX, FLT_MAX, 1.0f, cb);
		for (int i = 1; i < PxArticulationAxis::eCOUNT; i++)
			OverloadDoubleSpinBox(20 + i, w, ap[PxArticulationAxis::eTWIST + i].lowLimit, cb);
	}

	// High Limit
	{
		auto cb = [this](Id id, RVariant v) {
			auto axis = (PxArticulationAxis::Enum)(id - 30);
			GetChip()->SetHighLimit(axis, (PxReal)v.ToDouble());
			SetDirty();
		};
		auto *w = AddDoubleSpinBox(30, L"High Limit", ap[PxArticulationAxis::eTWIST].highLimit, -FLT_MAX, FLT_MAX, 1.0f, cb);
		for (int i = 1; i < PxArticulationAxis::eCOUNT; i++)
			OverloadDoubleSpinBox(30 + i, w, ap[PxArticulationAxis::eTWIST + i].highLimit, cb);
	}

	// Stiffness
	{
		auto cb = [this](Id id, RVariant v) {
			auto axis = (PxArticulationAxis::Enum)(id - 40);
			GetChip()->SetStiffness(axis, (PxReal)v.ToDouble());
			SetDirty();
		};
		auto *w = AddDoubleSpinBox(40, L"Stiffness", ap[PxArticulationAxis::eTWIST].stiffness, -FLT_MAX, FLT_MAX, 1.0f, cb);
		for (int i = 1; i < PxArticulationAxis::eCOUNT; i++)
			OverloadDoubleSpinBox(40 + i, w, ap[PxArticulationAxis::eTWIST + i].stiffness, cb);
	}

	// Damping
	{
		auto cb = [this](Id id, RVariant v) {
			auto axis = (PxArticulationAxis::Enum)(id - 50);
			GetChip()->SetDamping(axis, (PxReal)v.ToDouble());
			SetDirty();
		};
		auto *w = AddDoubleSpinBox(50, L"Damping", ap[PxArticulationAxis::eTWIST].damping, -FLT_MAX, FLT_MAX, 1.0f, cb);
		for (int i = 1; i < PxArticulationAxis::eCOUNT; i++)
			OverloadDoubleSpinBox(50 + i, w, ap[PxArticulationAxis::eTWIST + i].damping, cb);
	}

	// Max Force
	{
		auto cb = [this](Id id, RVariant v) {
			auto axis = (PxArticulationAxis::Enum)(id - 60);
			GetChip()->SetMaxForce(axis, (PxReal)v.ToDouble());
			SetDirty();
		};
		auto *w = AddDoubleSpinBox(60, L"Max Force", ap[PxArticulationAxis::eTWIST].maxForce, 0, FLT_MAX, 1.0f, cb);
		for (int i = 1; i < PxArticulationAxis::eCOUNT; i++)
			OverloadDoubleSpinBox(60 + i, w, ap[PxArticulationAxis::eTWIST + i].maxForce, cb);
	}

	// Is acceleration Drive
	{
		auto cb = [this](Id id, RVariant v) {
			auto axis = (PxArticulationAxis::Enum)(id - 70);
			GetChip()->SetIsAccelerationDrive(axis, v.ToUInt() == RCheckState::Checked);
			SetDirty();
		};
		auto *w = AddCheckBox(70, L"Is Acceleration Drive", ap[PxArticulationAxis::eTWIST].isAccelerationDrive ? RCheckState::Checked : RCheckState::Unchecked, cb);
		for (int i = 1; i < PxArticulationAxis::eCOUNT; i++)
			OverloadCheckBox(70 + i, w, ap[PxArticulationAxis::eTWIST + i].isAccelerationDrive ? RCheckState::Checked : RCheckState::Unchecked, cb);
	}

	// Drive Target
	{
		auto cb = [this](Id id, RVariant v) {
			auto axis = (PxArticulationAxis::Enum)(id - 80);
			GetChip()->SetDriveTarget(axis, (PxReal)v.ToDouble());
			SetDirty();
		};
		auto *w = AddDoubleSpinBox(80, L"Drive Target", ap[PxArticulationAxis::eTWIST].driveTarget, -FLT_MAX, FLT_MAX, 1.0f, cb);
		for (int i = 1; i < PxArticulationAxis::eCOUNT; i++)
			OverloadDoubleSpinBox(80 + i, w, ap[PxArticulationAxis::eTWIST + i].driveTarget, cb);
	}

	// Drive Target Vel
	{
		auto cb = [this](Id id, RVariant v) {
			auto axis = (PxArticulationAxis::Enum)(id - 90);
			GetChip()->SetMaxForce(axis, (PxReal)v.ToDouble());
			SetDirty();
		};
		auto *w = AddDoubleSpinBox(90, L"Drive Target Velocity", ap[PxArticulationAxis::eTWIST].driveTargetVel, 0, FLT_MAX, 1.0f, cb);
		for (int i = 1; i < PxArticulationAxis::eCOUNT; i++)
			OverloadDoubleSpinBox(90 + i, w, ap[PxArticulationAxis::eTWIST + i].driveTargetVel, cb);
	}
}

void PhysXArticulationJointRC_Dlg::_enableButtons()
{

}
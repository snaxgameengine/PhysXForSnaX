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
#include "PhysXValue.h"
#include "PhysXScene.h"
#include "PhysXActor.h"
#include "PhysXJoint.h"
#include "PhysXRigidDynamic.h"


using namespace m3d;


CHIPDESCV1_DEF(PhysXValue, L"PhysX Value", PHYSXVALUE_GUID, VALUE_GUID);


PhysXValue::PhysXValue()
{
	ClearConnections();
	_cmd = CMD_NONE;
}

PhysXValue::~PhysXValue()
{
}

bool PhysXValue::CopyChip(Chip *chip)
{
	PhysXValue *c = dynamic_cast<PhysXValue*>(chip);
	B_RETURN(Value::CopyChip(c));
	SetCommand(c->_cmd);
	return true;
}

bool PhysXValue::LoadChip(DocumentLoader &loader)
{
	B_RETURN(Value::LoadChip(loader));
	Command cmd;
	LOAD("cmd", (unsigned&)cmd);
	SetCommand(cmd);
	return true;
}

bool PhysXValue::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(Value::SaveChip(saver));
	SAVE("cmd", (unsigned)_cmd);
	return true;
}

value PhysXValue::GetValue()
{
	_value = 0.0f;
	switch (_cmd)
	{
	case CMD_SCENE_EXIST:
		{
			ChildPtr<PhysXScene> ch0 = GetChild(0);
			if (ch0)
				_value = ch0->Exist() ? 1.0f : 0.0f;
		}
		break;
	case CMD_SCENE_IS_RUNNING:
		{
			ChildPtr<PhysXScene> ch0 = GetChild(0);
			if (ch0)
				_value = ch0->IsRunning() ? 1.0f : 0.0f;
		}
		break;
	case CMD_SCENE_IS_SIMULATING:
		{
			ChildPtr<PhysXScene> ch0 = GetChild(0);
			if (ch0)
				_value = ch0->IsSimulating() ? 1.0f : 0.0f;
		}
		break;
	case CMD_SCENE_REAL_TIME_INDEX:
	{
		ChildPtr<PhysXScene> ch0 = GetChild(0);
		if (ch0)
			_value = ch0->GetRealTimeIndex();
		break;
	}
	case CMD_SCENE_SIM_INDEX:
	{
		ChildPtr<PhysXScene> ch0 = GetChild(0);
		if (ch0)
			_value = ch0->GetSimulationIndex();
		break;
	}
	case CMD_SCENE_SIM_WAIT:
	{
		ChildPtr<PhysXScene> ch0 = GetChild(0);
		if (ch0)
			_value = ch0->GetSimulationWaitingTime();
		break;
	}
	case CMD_ACTOR_EXIST:
		{
			ChildPtr<PhysXActor> ch0 = GetChild(0);
			if (ch0)
				_value = ch0->Exist() ? 1.0f : 0.0f;
		}
		break;
	case CMD_RIGID_BODY_XET_MASS:
		{
			ChildPtr<PhysXRigidBody> ch0 = GetChild(0);
			if (ch0) {
				PxRigidBody *a = ch0->GetRigidBody();
				if (a)
					_value = (value)a->getMass();
			}
		}
		break;
	case CMD_RIGID_DYNAMIC_XET_LINEAR_DAMPING:
		{
			ChildPtr<PhysXRigidDynamic> ch0 = GetChild(0);
			if (ch0) {
				PxRigidDynamic *a = ch0->GetRigidDynamic();
				if (a)
					_value = (value)a->getLinearDamping();
			}
		}
		break;
	case CMD_RIGID_DYNAMIC_XET_ANGULAR_DAMPING:
		{
			ChildPtr<PhysXRigidDynamic> ch0 = GetChild(0);
			if (ch0) {
				PxRigidDynamic *a = ch0->GetRigidDynamic();
				if (a)
					_value = (value)a->getAngularDamping();
			}
		}
		break;
	case CMD_RIGID_DYNAMIC_XET_MAX_ANGULAR_VELOCITY:
		{
			ChildPtr<PhysXRigidDynamic> ch0 = GetChild(0);
			if (ch0) {
				PxRigidDynamic *a = ch0->GetRigidDynamic();
				if (a)
					_value = (value)a->getMaxAngularVelocity();
			}
		}
		break;
	case CMD_RIGID_DYNAMIC_XET_SLEEP_THRESHOLD:
		{
			ChildPtr<PhysXRigidDynamic> ch0 = GetChild(0);
			if (ch0) {
				PxRigidDynamic *a = ch0->GetRigidDynamic();
				if (a)
					_value = (value)a->getSleepThreshold();
			}
		}
		break;
	case CMD_RIGID_DYNAMIC_IS_SLEEPING:
		{
			ChildPtr<PhysXRigidDynamic> ch0 = GetChild(0);
			if (ch0) {
				PxRigidDynamic *a = ch0->GetRigidDynamic();
				if (a)
					_value = a->isSleeping() ? 1.0f : 0.0f;
			}
		}
		break;
	case CMD_JOINT_EXIST:
		{
			ChildPtr<PhysXJoint> ch0 = GetChild(0);
			if (ch0)
				_value = ch0->Exist() ? 1.0f : 0.0f;
		}
		break;
	default:
		break;
	}
	return _value;
}

void PhysXValue::SetValue(value v)
{
	switch (_cmd)
	{
	case CMD_RIGID_BODY_XET_MASS:
		{
			ChildPtr<PhysXRigidBody> ch0 = GetChild(0);
			if (ch0) {
				PxRigidBody *a = ch0->GetRigidBody();
				if (a) {
					a->setMass(PxReal(_value = __max(_value, 1.0e-6f)));
					PxVec3 v = a->getCMassLocalPose().p;
					PxRigidBodyExt::setMassAndUpdateInertia(*a, a->getMass(), &v);
				}
			}
		}
		break;
	case CMD_RIGID_DYNAMIC_XET_LINEAR_DAMPING:
		{
			ChildPtr<PhysXRigidDynamic> ch0 = GetChild(0);
			if (ch0) {
				PxRigidDynamic *a = ch0->GetRigidDynamic();
				if (a)
					a->setLinearDamping(PxReal(_value = __max(_value, 0.0f)));
			}
		}
		break;
	case CMD_RIGID_DYNAMIC_XET_ANGULAR_DAMPING:
		{
			ChildPtr<PhysXRigidDynamic> ch0 = GetChild(0);
			if (ch0) {
				PxRigidDynamic *a = ch0->GetRigidDynamic();
				if (a)
					a->setAngularDamping(PxReal(_value = __max(_value, 0.0f)));
			}
		}
		break;
	case CMD_RIGID_DYNAMIC_XET_MAX_ANGULAR_VELOCITY:
		{
			ChildPtr<PhysXRigidDynamic> ch0 = GetChild(0);
			if (ch0) {
				PxRigidDynamic *a = ch0->GetRigidDynamic();
				if (a)
					a->setAngularDamping(PxReal(_value = __max(_value, 1.0e-6f)));
			}
		}
		break;
	case CMD_RIGID_DYNAMIC_XET_SLEEP_THRESHOLD:
		{
			ChildPtr<PhysXRigidDynamic> ch0 = GetChild(0);
			if (ch0) {
				PxRigidDynamic *a = ch0->GetRigidDynamic();
				if (a)
					a->setAngularDamping(PxReal(_value = __max(_value, 1.0e-6f)));
			}
		}
		break;
	default:
		break;
	}
}

void PhysXValue::SetCommand(Command cmd)
{
	if (_cmd == cmd)
		return;
	
	_cmd = cmd;

	switch (_cmd)
	{
	case CMD_SCENE_EXIST:
	case CMD_SCENE_IS_RUNNING:
	case CMD_SCENE_IS_SIMULATING:
	case CMD_SCENE_REAL_TIME_INDEX:
	case CMD_SCENE_SIM_INDEX:
	case CMD_SCENE_SIM_WAIT:
		CREATE_CHILD_KEEP(0, PHYSXSCENE_GUID, false, UP, L"Scene");
		ClearConnections(1);
		break;
	case CMD_ACTOR_EXIST:
		CREATE_CHILD_KEEP(0, PHYSXACTOR_GUID, false, UP, L"Actor");
		ClearConnections(1);
		break;
	case CMD_RIGID_BODY_XET_MASS:
		CREATE_CHILD_KEEP(0, PHYSXRIGIDBODY_GUID, false, BOTH, L"Actor");
		ClearConnections(1);
		break;
	case CMD_RIGID_DYNAMIC_XET_LINEAR_DAMPING:
	case CMD_RIGID_DYNAMIC_XET_ANGULAR_DAMPING:
	case CMD_RIGID_DYNAMIC_XET_MAX_ANGULAR_VELOCITY:
	case CMD_RIGID_DYNAMIC_XET_SLEEP_THRESHOLD:
		CREATE_CHILD_KEEP(0, PHYSXRIGIDDYNAMIC_GUID, false, BOTH, L"Actor");
		ClearConnections(1);
		break;
	case CMD_RIGID_DYNAMIC_IS_SLEEPING:
		CREATE_CHILD_KEEP(0, PHYSXRIGIDDYNAMIC_GUID, false, UP, L"Actor");
		ClearConnections(1);
		break;
	case CMD_JOINT_EXIST:
		CREATE_CHILD_KEEP(0, PHYSXJOINT_GUID, false, UP, L"Joint");
		ClearConnections(1);
		break;
	default:
		ClearConnections();
		break;
	}
}
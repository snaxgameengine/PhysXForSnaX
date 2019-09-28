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
#include "PhysXVector.h"
#include "PhysXScene.h"
#include "PhysXRigidBody.h"
#include "PhysXJoint.h"

using namespace m3d;


CHIPDESCV1_DEF(PhysXVector, L"PhysX Vector", PHYSXVECTOR_GUID, VECTOR_GUID);


PhysXVector::PhysXVector()
{
	ClearConnections();
	_cmd = CMD_NONE;
}

PhysXVector::~PhysXVector()
{
}

bool PhysXVector::CopyChip(Chip *chip)
{
	PhysXVector *c = dynamic_cast<PhysXVector*>(chip);
	B_RETURN(Vector::CopyChip(c));
	SetCommand(c->_cmd);
	return true;
}

bool PhysXVector::LoadChip(DocumentLoader &loader)
{
	B_RETURN(Vector::LoadChip(loader));
	Command cmd;
	LOAD("cmd", (unsigned&)cmd);
	SetCommand(cmd);
	return true;
}

bool PhysXVector::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(Vector::SaveChip(saver));
	SAVE("cmd", (unsigned)_cmd);
	return true;
}

const XMFLOAT4 &PhysXVector::GetVector()
{
	_vector = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	switch (_cmd)
	{
	case CMD_SCENE_XET_GRAVITY:
		{
			ChildPtr<PhysXScene> ch0 = GetChild(0);
			if (ch0) {
				PxScene *s = ch0->GetScene();
				if (s) 
					_vector = (XMFLOAT4&)PxVec4(s->getGravity(), 0.0f);
			}
		}
		break;
	case CMD_RIGID_BODY_XET_LINEAR_VELOCITY:
		{
			ChildPtr<PhysXRigidBody> ch0 = GetChild(0);
			if (ch0) {
				PxRigidBody *a = ch0->GetRigidBody();
				if (a) 
					_vector = (XMFLOAT4&)PxVec4(a->getLinearVelocity(), 0.0f);
			}
		}
		break;
	case CMD_RIGID_BODY_XET_ANGULAR_VELOCITY:
		{
			ChildPtr<PhysXRigidBody> ch0 = GetChild(0);
			if (ch0) {
				PxRigidBody *a = ch0->GetRigidBody();
				if (a) 
					_vector = (XMFLOAT4&)PxVec4(a->getAngularVelocity(), 0.0f);
			}
		}
		break;
	case CMD_RIGID_BODY_XET_MASS_SPACE_INERTIA_TENSOR:
		{
			ChildPtr<PhysXRigidBody> ch0 = GetChild(0);
			if (ch0) {
				PxRigidBody *a = ch0->GetRigidBody();
				if (a) 
					_vector = (XMFLOAT4&)PxVec4(a->getMassSpaceInertiaTensor(), 0.0f);
			}
		}
		break;
	case CMD_CONSTRAINT_GET_LINEAR_FORCE:
	case CMD_CONSTRAINT_GET_ANGULAR_FORCE:
		{
			ChildPtr<PhysXJoint> ch0 = GetChild(0);
			if (ch0) {
				PxJoint *j = ch0->GetJoint();
				if (j) {
					PxConstraint *c = j->getConstraint(); // Note: This is somewhat "unofficial" as the getConstraint()-method is declared private. I changed it to public to make this work! :) 
					if (c) {
						XMFLOAT4 l(0.0f, 0.0f, 0.0f, 0.0f), a(0.0f, 0.0f, 0.0f, 0.0f);
						c->getForce((PxVec3&)l, (PxVec3&)a);
						_vector = _cmd == CMD_CONSTRAINT_GET_LINEAR_FORCE ? l : a;
					}
				}
			}
		}
		break;
	default:
		break;
	}
	return _vector;
}

void PhysXVector::SetVector(const XMFLOAT4 &v)
{
	switch (_cmd)
	{
	case CMD_SCENE_XET_GRAVITY:
		{
			ChildPtr<PhysXScene> ch0 = GetChild(0);
			if (ch0) {
				PxScene *s = ch0->GetScene();
				if (s)
					s->setGravity((PxVec3&)(_vector = v));
			}
		}
		break;
	case CMD_RIGID_BODY_XET_LINEAR_VELOCITY:
		{
			ChildPtr<PhysXRigidBody> ch0 = GetChild(0);
			if (ch0) {
				PxRigidBody *a = ch0->GetRigidBody();
				if (a)
					a->setLinearVelocity((PxVec3&)(_vector = v));
			}
		}
		break;
	case CMD_RIGID_BODY_XET_ANGULAR_VELOCITY:
		{
			ChildPtr<PhysXRigidBody> ch0 = GetChild(0);
			if (ch0) {
				PxRigidBody *a = ch0->GetRigidBody();
				if (a)
					a->setAngularVelocity((PxVec3&)(_vector = v));
			}
		}
		break;
	case CMD_RIGID_BODY_XET_MASS_SPACE_INERTIA_TENSOR:
		{
			ChildPtr<PhysXRigidBody> ch0 = GetChild(0);
			if (ch0) {
				PxRigidBody *a = ch0->GetRigidBody();
				if (a)
					a->setMassSpaceInertiaTensor((PxVec3&)(_vector = v));
			}
		}
		break;
	default:
		break;
	}
}

void PhysXVector::SetCommand(Command cmd)
{
	if (_cmd == cmd)
		return;
	
	_cmd = cmd;
	
	switch (_cmd)
	{
	case CMD_SCENE_XET_GRAVITY:
		CREATE_CHILD_KEEP(0, PHYSXSCENE_GUID, false, BOTH, L"Scene");
		ClearConnections(1);
		break;
	case CMD_RIGID_BODY_XET_LINEAR_VELOCITY:
	case CMD_RIGID_BODY_XET_ANGULAR_VELOCITY:
	case CMD_RIGID_BODY_XET_MASS_SPACE_INERTIA_TENSOR:
		CREATE_CHILD_KEEP(0, PHYSXRIGIDBODY_GUID, false, BOTH, L"Actor");
		ClearConnections(1);
		break;
	case CMD_CONSTRAINT_GET_LINEAR_FORCE:
	case CMD_CONSTRAINT_GET_ANGULAR_FORCE:
		CREATE_CHILD_KEEP(0, PHYSXJOINT_GUID, false, UP, L"Joint");
		ClearConnections(1);
		break;
	default:
		ClearConnections();
		break;
	}
}
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
#include "PhysXCommand.h"
#include "PhysXScene.h"
#include "GraphicsChips/DebugGeometry.h"
#include "PhysXRigidDynamic.h"
#include "StdChips/Vector.h"
#include "PhysXJoint.h"


using namespace m3d;


CHIPDESCV1_DEF(PhysXCommand, L"PhysX Command", PHYSXCOMMAND_GUID, CHIP_GUID);


PhysXCommand::PhysXCommand()
{
	_cmd = CMD_NONE;
}

PhysXCommand::~PhysXCommand()
{
}

bool PhysXCommand::CopyChip(Chip *chip)
{
	PhysXCommand *c = dynamic_cast<PhysXCommand*>(chip);
	B_RETURN(Chip::CopyChip(c));
	SetCommand(c->_cmd);
	return true;
}

bool PhysXCommand::LoadChip(DocumentLoader &loader)
{
	B_RETURN(Chip::LoadChip(loader));
	Command cmd;
	LOAD("cmd", (unsigned&)cmd);
	SetCommand(cmd);
	return true;
}

bool PhysXCommand::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(Chip::SaveChip(saver));
	SAVE("cmd", (unsigned)_cmd);
	return true;
}

void PhysXCommand::CallChip()
{
	switch (_cmd)
	{
	case CMD_SCENE_SIMULATE:
		{
			ChildPtr<PhysXScene> chScene = GetChild(0);
			if (chScene)
				chScene->Simulate();
		}
		break;
	case CMD_SCENE_FETCH_RESULTS:
		{
			ChildPtr<PhysXScene> chScene = GetChild(0);
			if (chScene)
				chScene->FetchResults();
		}
		break;
	case CMD_SCENE_DEBUG_RENDER:
		{
			ChildPtr<PhysXScene> chScene = GetChild(0);
			if (chScene) {
				PxScene *scene = chScene->GetScene();

				DebugGeometry* dg = dynamic_cast<DebugGeometry*>(engine->GetChipManager()->GetGlobalChip(DEBUGGEOMETRY_GUID));

				const PxRenderBuffer &buffer = scene->getRenderBuffer();

				for (unsigned i = 0; i < buffer.getNbPoints(); i++) {
					const PxDebugPoint &p = buffer.getPoints()[i];
					dg->AddPoint(DebugVertex(XMFLOAT3((float*)&p.pos), XMUBYTE4(p.color)));
				}

				for (unsigned i = 0; i < buffer.getNbLines(); i++) {
					const PxDebugLine &line = buffer.getLines()[i];
					dg->AddLineSegment(DebugVertex(XMFLOAT3((float*)&line.pos0), XMUBYTE4(line.color0)), DebugVertex(XMFLOAT3((float*)&line.pos1), XMUBYTE4(line.color1)));
				}

				for (unsigned i = 0; i < buffer.getNbTriangles(); i++) {
					const PxDebugTriangle &triangle = buffer.getTriangles()[i];
					dg->AddTriangle(DebugVertex(XMFLOAT3((float*)&triangle.pos0), XMUBYTE4(triangle.color0)), DebugVertex(XMFLOAT3((float*)&triangle.pos1), XMUBYTE4(triangle.color1)), DebugVertex(XMFLOAT3((float*)&triangle.pos2), XMUBYTE4(triangle.color2)));
				}
			}	
		}
		break;
	case CMD_SCENE_START:
		{
			ChildPtr<PhysXScene> chScene = GetChild(0);
			if (chScene)
				chScene->StartSimulation();
		}
		break;
	case CMD_SCENE_STOP:
		{
			ChildPtr<PhysXScene> chScene = GetChild(0);
			if (chScene)
				chScene->StopSimulation();
		}
		break;
	case CMD_SCENE_DESTROY:
		{
			ChildPtr<PhysXScene> chScene = GetChild(0);
			if (chScene)
				chScene->DestroyScene();
		}
		break;
	case CMD_ACTOR_DESTROY:
		{
			ChildPtr<PhysXActor> chActor = GetChild(0);
			if (chActor)
				chActor->DestroyActor();
		}
		break;
	case CMD_RIGIDDYNAMIC_ADD_GLOBAL_FORCE_AT_GLOBAL_POINT:
	case CMD_RIGIDDYNAMIC_ADD_GLOBAL_FORCE_AT_LOCAL_POINT:
	case CMD_RIGIDDYNAMIC_ADD_LOCAL_FORCE_AT_GLOBAL_POINT:
	case CMD_RIGIDDYNAMIC_ADD_LOCAL_FORCE_AT_LOCAL_POINT:
		{
			ChildPtr<PhysXRigidDynamic> chActor = GetChild(0);
			ChildPtr<Vector> chForce = GetChild(1);
			ChildPtr<Vector> chPos = GetChild(2);
			if (chActor && chForce) {
				XMFLOAT4 f = chForce->GetVector();
				XMFLOAT4 p = chPos ? chPos->GetVector() : XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
				chActor->AddForce(PhysXForce(
					_cmd == CMD_RIGIDDYNAMIC_ADD_GLOBAL_FORCE_AT_GLOBAL_POINT ? PhysXForce::GLOBAL_AT_GLOBAL_POS : (_cmd == CMD_RIGIDDYNAMIC_ADD_GLOBAL_FORCE_AT_LOCAL_POINT ? PhysXForce::GLOBAL_AT_LOCAL_POS : 
					(_cmd == CMD_RIGIDDYNAMIC_ADD_LOCAL_FORCE_AT_GLOBAL_POINT ? PhysXForce::LOCAL_AT_GLOBAL_POS : PhysXForce::LOCAL_AT_LOCAL_POS)), PxVec3(f.x,f.y,f.z), PxVec3(p.x, p.y, p.z)));
			}
		}
		break;
	case CMD_RIGIDDYNAMIC_ADD_GLOBAL_TORQUE:
	case CMD_RIGIDDYNAMIC_ADD_LOCAL_TORQUE:
		{
			ChildPtr<PhysXRigidDynamic> chActor = GetChild(0);
			ChildPtr<Vector> chForce = GetChild(1);
			if (chActor && chForce) {
				XMFLOAT4 f = chForce->GetVector();
				chActor->AddForce(PhysXForce(_cmd == CMD_RIGIDDYNAMIC_ADD_GLOBAL_TORQUE ? PhysXForce::GLOBAL_TORQUE : PhysXForce::LOCAL_TORQUE, PxVec3(f.x, f.y, f.z)));
			}
		}
		break;
	case CMD_RIGIDDYNAMIC_CLEAR_FORCES_AND_TORQUES:
		{
			ChildPtr<PhysXRigidDynamic> chActor = GetChild(0);
			if (chActor)
				chActor->ClearForces();
		}
		break;
	case CMD_JOINT_DESTROY:
		{
			ChildPtr<PhysXJoint> chJoint = GetChild(0);
			if (chJoint)
				chJoint->DestroyJoint();
		}
		break;
	}
}

void PhysXCommand::SetCommand(Command cmd)
{
	if (_cmd == cmd)
		return;
	
	_cmd = cmd;

	switch (_cmd)
	{
	case CMD_SCENE_SIMULATE:
	case CMD_SCENE_FETCH_RESULTS:
	case CMD_SCENE_DEBUG_RENDER:
	case CMD_SCENE_START:
	case CMD_SCENE_STOP:
	case CMD_SCENE_DESTROY:
		CREATE_CHILD_KEEP(0, PHYSXSCENE_GUID, false, UP, L"Scene");
		ClearConnections(1);
		break;
	case CMD_ACTOR_DESTROY:
		CREATE_CHILD_KEEP(0, PHYSXACTOR_GUID, false, DOWN, L"Actor");
		ClearConnections(1);
		break;
	case CMD_RIGIDDYNAMIC_ADD_GLOBAL_FORCE_AT_GLOBAL_POINT:
	case CMD_RIGIDDYNAMIC_ADD_GLOBAL_FORCE_AT_LOCAL_POINT:
	case CMD_RIGIDDYNAMIC_ADD_LOCAL_FORCE_AT_GLOBAL_POINT:
	case CMD_RIGIDDYNAMIC_ADD_LOCAL_FORCE_AT_LOCAL_POINT:
		CREATE_CHILD_KEEP(0, PHYSXRIGIDDYNAMIC_GUID, false, DOWN, L"Rigid Dynamic");
		CREATE_CHILD_KEEP(1, VECTOR_GUID, false, UP, L"Force");
		CREATE_CHILD_KEEP(2, VECTOR_GUID, false, UP, L"Position");
		ClearConnections(3);
		break;
	case CMD_RIGIDDYNAMIC_ADD_GLOBAL_TORQUE:
	case CMD_RIGIDDYNAMIC_ADD_LOCAL_TORQUE:
		CREATE_CHILD_KEEP(0, PHYSXRIGIDDYNAMIC_GUID, false, DOWN, L"Rigid Dynamic");
		CREATE_CHILD_KEEP(1, VECTOR_GUID, false, UP, L"Torque");
		ClearConnections(2);
		break;
	case CMD_RIGIDDYNAMIC_CLEAR_FORCES_AND_TORQUES:
		CREATE_CHILD_KEEP(0, PHYSXRIGIDDYNAMIC_GUID, false, DOWN, L"Rigid Dynamic");
		ClearConnections(1);
		break;
	case CMD_JOINT_DESTROY:
		CREATE_CHILD_KEEP(0, PHYSXJOINT_GUID, false, DOWN, L"Joint");
		ClearConnections(1);
		break;
	default:
		ClearConnections();
		break;
	}
}
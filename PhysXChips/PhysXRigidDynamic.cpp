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
#include "PhysXRigidDynamic.h"
#include "PhysXScene.h"
#include "PhysXShape.h"
#include "StdChips/MatrixChip.h"
#include "StdChips/Value.h"
#include "StdChips/Vector.h"

using namespace m3d;


CHIPDESCV1_DEF(PhysXRigidDynamic, L"PhysX Rigid Dynamic", PHYSXRIGIDDYNAMIC_GUID, PHYSXRIGIDBODY_GUID);


PhysXRigidDynamic::PhysXRigidDynamic()
{
	CREATE_CHILD(0, PHYSXSCENE_GUID, false, BOTH, L"Scene");
	CREATE_CHILD(1, MATRIXCHIP_GUID, false, UP, L"Transform");
	CREATE_CHILD(2, VALUE_GUID, false, UP, L"Mass");
	CREATE_CHILD(3, VECTOR_GUID, false, UP, L"Center of Mass");
	CREATE_CHILD(4, PHYSXSHAPE_GUID, true, UP, L"Shapes");

	_actor = nullptr;

	_kinematic = false;
	_linearDamping = 0.0f;
	_angularDamping = 0.0f;
	_maxAngularVelocity = 7.0f;
	_sleepThreshold = 0.05f;
	_minPositionIters = 4;
	_minVelocityIters = 1;
	_contactReportThreshold = FLT_MAX;
}

PhysXRigidDynamic::~PhysXRigidDynamic()
{
}

bool PhysXRigidDynamic::CopyChip(Chip *chip)
{
	PhysXRigidDynamic *c = dynamic_cast<PhysXRigidDynamic*>(chip);
	B_RETURN(PhysXRigidBody::CopyChip(c));
	_kinematic = c->_kinematic;
	_linearDamping = c->_linearDamping;
	_angularDamping = c->_angularDamping;
	_maxAngularVelocity = c->_maxAngularVelocity;
	_sleepThreshold = c->_sleepThreshold;
	_minPositionIters = c->_minPositionIters;
	_minVelocityIters = c->_minVelocityIters;
	_contactReportThreshold = c->_contactReportThreshold;
	return true;
}

bool PhysXRigidDynamic::LoadChip(DocumentLoader &loader)
{
	B_RETURN(PhysXRigidBody::LoadChip(loader));
	LOADDEF("kinematic", _kinematic, false);
	LOADDEF("linearDamping", _linearDamping, 0.0f);
	LOADDEF("angularDamping", _angularDamping, 0.0f);
	LOADDEF("maxAngularVelocity", _maxAngularVelocity, 7.0f);
	LOADDEF("sleepThreshold", _sleepThreshold, 0.05f);
	LOADDEF("minPositionIters", _minPositionIters, 4);
	LOADDEF("minVelocityIters", _minVelocityIters, 1);
	LOADDEF("contactReportThreshold", _contactReportThreshold, FLT_MAX);
	return true;
}

bool PhysXRigidDynamic::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(PhysXRigidBody::SaveChip(saver));
	SAVEDEF("kinematic", _kinematic, false);
	SAVEDEF("linearDamping", _linearDamping, 0.0f);
	SAVEDEF("angularDamping", _angularDamping, 0.0f);
	SAVEDEF("maxAngularVelocity", _maxAngularVelocity, 7.0f);
	SAVEDEF("sleepThreshold", _sleepThreshold, 0.05f);
	SAVEDEF("minPositionIters", _minPositionIters, 4);
	SAVEDEF("minVelocityIters", _minVelocityIters, 1);
	SAVEDEF("contactReportThreshold", _contactReportThreshold, FLT_MAX);
	return true;
}

void PhysXRigidDynamic::CallChip()
{
	if (Exist()) {
		// Update shapes?
		return;
	}

	PxTransform transform = PxTransform(PxIdentity);
	
	ChildPtr<MatrixChip> chTransform = GetChild(1);
	if (chTransform) {
		transform = PxTransform(PxMat44((float*)&chTransform->GetMatrix()));
	}

	_actor = CreateActor(transform);

	if (_actor)
		((PhysXScene*)_actor->getScene()->userData)->RegisterSceneObject(this);
}

PxRigidDynamic *PhysXRigidDynamic::CreateActor(const PxTransform &pose)
{
	ChildPtr<PhysXScene> chScene = GetChild(0);
	if (!chScene)
		return nullptr;

	PxScene *scene = chScene->GetScene();
	if (!scene)
		return nullptr;

	PxRigidDynamic *actor = scene->getPhysics().createRigidDynamic(pose);
	if (!actor)
		return nullptr;

	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, _disableGravity);
	actor->setActorFlag(PxActorFlag::eSEND_SLEEP_NOTIFIES, _sendSleepNotifies);
	actor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, _kinematic);
	actor->setAngularDamping(_angularDamping);
	actor->setLinearDamping(_linearDamping);
	actor->setMaxAngularVelocity(_maxAngularVelocity);
	actor->setSolverIterationCounts(_minPositionIters, _minVelocityIters);
	actor->setSleepThreshold(_sleepThreshold);
	actor->setContactReportThreshold(_contactReportThreshold);

	List<PxReal> shapeMasses;
	
	for (unsigned i = 0; i < GetSubConnectionCount(4); i++) {
		ChildPtr<PhysXShape> ch2 = GetChild(4, i);
		if (ch2) {
			PxShape *shape = ch2->GetShape();
			if (shape) {
				actor->attachShape(*shape);
				PxReal mass = ch2->GetMass();
				shapeMasses.push_back(mass);
			}
		}
	}

	float mass = 0.0f;
	ChildPtr<Value> chMass = GetChild(2);
	if (chMass)
		mass = (float)chMass->GetValue();
	mass = __max(mass, 0.0f);

	PxVec3 com(0.0f, 0.0f, 0.0f);
	ChildPtr<Vector> chCOM = GetChild(3);
	if (chCOM) {
		XMFLOAT4 v = chCOM->GetVector();
		com = PxVec3(v.x, v.y, v.z);
	}

	if (mass == 0.0f) {
		bool b = PxRigidBodyExt::setMassAndUpdateInertia(*actor, &shapeMasses.front(), (PxU32)shapeMasses.size(), &com);
	}
	else {
		bool b = PxRigidBodyExt::setMassAndUpdateInertia(*actor, mass, &com);
	}
	//	PxRigidBodyExt::updateMassAndInertia(*actor, 10);

		
	PhysXActorData *data = mmnew PhysXActorData(actor);
	data->globalPose = actor->getGlobalPose();
	actor->userData = data;

	scene->addActor(*actor);

	return actor;
}

void PhysXRigidDynamic::DestroyActor() 
{
	if (_actor) {
		if (_actor->getScene())
			((PhysXScene*)_actor->getScene()->userData)->UnregisterSceneObject(this);
		mmdelete((PhysXActorData*)_actor->userData);
		_actor->release();
		_actor = nullptr;
	}
}

void PhysXRigidDynamic::AddForce(const PhysXForce &force)
{
	PxRigidActor *actor = GetRigidActor();
	if (!actor)
		return;
	PhysXActorData *data = (PhysXActorData*)actor->userData;
	data->forces.push_back(force);
}

void PhysXRigidDynamic::ClearForces()
{
	PxRigidActor *actor = GetRigidActor();
	if (!actor)
		return;
	PhysXActorData *data = (PhysXActorData*)actor->userData;
	data->forces.clear();
}

bool PhysXRigidDynamic::IsKinematic()
{
	PxRigidDynamic *a = GetRigidDynamic();
	if (a)
		return a->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC;
	return _kinematic;
}

float PhysXRigidDynamic::GetLinearDamping()
{
	PxRigidDynamic *a = GetRigidDynamic();
	if (a)
		return a->getLinearDamping();
	return _linearDamping;
}

float PhysXRigidDynamic::GetAngularDamping()
{
	PxRigidDynamic *a = GetRigidDynamic();
	if (a)
		return a->getAngularDamping();
	return _angularDamping;
}

float PhysXRigidDynamic::GetMaxAngularVelocity()
{
	PxRigidDynamic *a = GetRigidDynamic();
	if (a)
		return a->getMaxAngularVelocity();
	return _maxAngularVelocity;
}

float PhysXRigidDynamic::GetSleepThreshold()
{
	PxRigidDynamic *a = GetRigidDynamic();
	if (a)
		return a->getSleepThreshold();
	return _sleepThreshold;
}

void PhysXRigidDynamic::GetMinSolverIterations(unsigned &pos, unsigned &vel)
{
	PxRigidDynamic *a = GetRigidDynamic();
	if (a)
		return a->getSolverIterationCounts(pos, vel);
	pos = _minPositionIters;
	vel = _minVelocityIters;
}

float PhysXRigidDynamic::GetContactReportThreshold()
{
	PxRigidDynamic *a = GetRigidDynamic();
	if (a)
		return a->getContactReportThreshold();
	return _contactReportThreshold;
}

void PhysXRigidDynamic::SetKinematic(bool k, bool chipSettings)
{
	PxRigidDynamic *a = GetRigidDynamic();
	if (a)
		a->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, k);
	if (chipSettings)
		_kinematic = k;
}

void PhysXRigidDynamic::SetLinearDamping(float f, bool chipSettings)
{
	PxRigidDynamic *a = GetRigidDynamic();
	if (a)
		a->setLinearDamping(f);
	if (chipSettings)
		_linearDamping = f;
}

void PhysXRigidDynamic::SetAngularDamping(float f, bool chipSettings)
{
	PxRigidDynamic *a = GetRigidDynamic();
	if (a)
		a->setAngularDamping(f);
	if (chipSettings)
		_angularDamping = f;
}

void PhysXRigidDynamic::SetMaxAngularVelocity(float f, bool chipSettings)
{
	PxRigidDynamic *a = GetRigidDynamic();
	if (a)
		a->setMaxAngularVelocity(f);
	if (chipSettings)
		_maxAngularVelocity = f;
}

void PhysXRigidDynamic::SetSleepThreshold(float f, bool chipSettings)
{
	PxRigidDynamic *a = GetRigidDynamic();
	if (a)
		a->setSleepThreshold(f);
	if (chipSettings)
		_sleepThreshold = f;
}

void PhysXRigidDynamic::SetMinSolverIterations(unsigned pos, unsigned vel, bool chipSettings)
{
	PxRigidDynamic *a = GetRigidDynamic();
	if (a)
		a->setSolverIterationCounts(pos, vel);
	if (chipSettings) {
		_minPositionIters = pos;
		_minVelocityIters = vel;
	}
}

void PhysXRigidDynamic::SetContactReportThreshold(float f, bool chipSettings)
{
	PxRigidDynamic *a = GetRigidDynamic();
	if (a)
		a->setContactReportThreshold(f);
	if (chipSettings)
		_contactReportThreshold = f;
}




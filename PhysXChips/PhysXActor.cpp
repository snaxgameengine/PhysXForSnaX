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
#include "PhysXRigidActor.h"

using namespace m3d;


CHIPDESCV1_DEF(PhysXActor, L"PhysX Actor", PHYSXACTOR_GUID, CHIP_GUID);


PhysXActor::PhysXActor()
{
	_disableGravity = false;
	_sendSleepNotifies = false;
	_visualization = true;
	_dominanceGroup = 0;
}

PhysXActor::~PhysXActor()
{
}

bool PhysXActor::CopyChip(Chip *chip)
{
	PhysXActor *c = dynamic_cast<PhysXActor*>(chip);
	B_RETURN(Chip::CopyChip(c));
	_disableGravity = c->_disableGravity;
	_sendSleepNotifies = c->_sendSleepNotifies;
	_visualization = c->_visualization;
	_dominanceGroup = c->_dominanceGroup;
	return true;
}

bool PhysXActor::LoadChip(DocumentLoader &loader)
{
	B_RETURN(Chip::LoadChip(loader));
	LOADDEF("disableGravity", _disableGravity, false);
	LOADDEF("sendSleepNotifies", _sendSleepNotifies, false);
	LOADDEF("visualization", _visualization, true);
	LOADDEF("dominanceGroup", _dominanceGroup, 0);
	return true;
}

bool PhysXActor::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(Chip::SaveChip(saver));
	SAVEDEF("disableGravity", _disableGravity, false);
	SAVEDEF("sendSleepNotifies", _sendSleepNotifies, false);
	SAVEDEF("visualization", _visualization, true);
	SAVEDEF("dominanceGroup", _dominanceGroup, 0);
	return true;
}

bool PhysXActor::Exist() const
{
	PxActor *a = GetActor();
	assert(!a || a->getScene());
	return a != nullptr;
}

bool PhysXActor::IsDisableGravity() const
{
	PxActor *a = GetActor();
	if (a)
		return a->getActorFlags() == PxActorFlag::eDISABLE_GRAVITY;
	return _disableGravity;
}

bool PhysXActor::IsSendSleepNotifies() const
{
	PxActor *a = GetActor();
	if (a)
		return a->getActorFlags() == PxActorFlag::eSEND_SLEEP_NOTIFIES;
	return _sendSleepNotifies;
}

bool PhysXActor::IsVisualization() const
{
	PxActor *a = GetActor();
	if (a)
		return a->getActorFlags() == PxActorFlag::eVISUALIZATION;
	return _visualization;
}

PxDominanceGroup PhysXActor::GetDominanceGroup() const
{
	PxActor *a = GetActor();
	if (a)
		a->getDominanceGroup();
	return _dominanceGroup;
}

void PhysXActor::SetDisableGravity(bool b, bool chipSettings)
{
	PxActor *a = GetActor();
	if (a)
		a->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, b);
	if (chipSettings)
		_disableGravity = b;
}

void PhysXActor::SetSendSleepNotifies(bool b, bool chipSettings)
{
	PxActor *a = GetActor();
	if (a)
		a->setActorFlag(PxActorFlag::eSEND_SLEEP_NOTIFIES, b);
	if (chipSettings)
		_sendSleepNotifies = b;
}

void PhysXActor::SetVisualization(bool b, bool chipSettings)
{
	PxActor *a = GetActor();
	if (a)
		a->setActorFlag(PxActorFlag::eVISUALIZATION, b);
	if (chipSettings)
		_visualization = b;
}

void PhysXActor::SetDominanceGroup(PxDominanceGroup b, bool chipSettings)
{
	PxActor *a = GetActor();
	if (a)
		a->setDominanceGroup(b);
	if (chipSettings)
		_dominanceGroup = b;
}

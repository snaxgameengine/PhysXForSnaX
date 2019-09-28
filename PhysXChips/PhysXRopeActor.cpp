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
#include "PhysXRopeActor.h"
#include "PhysXRope.h"
#include "StdChips/Value.h"


using namespace m3d;


CHIPDESCV1_DEF(PhysXRopeActor, L"PhysX Rope Actor", PHYSXROPEACTOR_GUID, PHYSXRIGIDDYNAMIC_GUID);


PhysXRopeActor::PhysXRopeActor()
{
	ClearConnections();

	_type = NONE;
}

PhysXRopeActor::~PhysXRopeActor()
{
}

bool PhysXRopeActor::CopyChip(Chip *chip)
{
	PhysXRopeActor *c = dynamic_cast<PhysXRopeActor*>(chip);
	B_RETURN(Chip::CopyChip(c)); // Skip PhysXRigidDynamic
	SetType(c->_type);
	return true;
}

bool PhysXRopeActor::LoadChip(DocumentLoader &loader)
{
	B_RETURN(Chip::LoadChip(loader)); // Skip PhysXRigidDynamic
	Type type;
	LOAD("type", (unsigned&)type);
	SetType(type);
	return true;
}

bool PhysXRopeActor::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(Chip::SaveChip(saver)); // Skip PhysXRigidDynamic
	SAVE("type", (unsigned)_type);
	return true;
}

PxRigidDynamic *PhysXRopeActor::GetRigidDynamic() const
{
	ChildPtr<PhysXRope> chRope = GetChild(0);
	if (!chRope)
		return nullptr;

	unsigned index = -1;

	switch (_type)
	{
	case FIRST:
		index = 0;
		break;
	case LAST:
		index = (unsigned)chRope->GetActors().size() - 1;
		break;
	case INDEX:
		{
			ChildPtr<Value> chIndex = GetChild(1);
			if (chIndex)
				index = (unsigned)chIndex->GetValue();
		}
		break;
	}
	
	const List<PxRigidDynamic*> &actors = chRope->GetActors();
	if (index < actors.size())
		return actors[index];

	return nullptr;
}

void PhysXRopeActor::SetType(Type type)
{
	if (_type == type)
		return;

	_type = type;

	CREATE_CHILD_KEEP(0, PHYSXROPE_GUID, false, UP, L"Rope");

	switch (_type)
	{
	case FIRST:
	case LAST:
	case NONE:
		ClearConnections(1);
		break;
	case INDEX:
		CREATE_CHILD_KEEP(1, VALUE_GUID, false, UP, L"Actor Index");
	}
}
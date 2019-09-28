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
#include "PhysXArticulationJointBase.h"
#include "PhysXArticulationLink.h"
#include "StdChips/MatrixChip.h"

using namespace m3d;


CHIPDESCV1_DEF_HIDDEN(PhysXArticulationJointBase, L"PhysX Articulation Joint Base", PHYSXARTICULATIONJOINTBASE_GUID, CHIP_GUID);



PhysXArticulationJointBase::PhysXArticulationJointBase()
{
	CREATE_CHILD(0, PHYSXARTICULATIONLINK_GUID, false, BOTH, L"Link");

	_globalFrame = false;
}

PhysXArticulationJointBase::~PhysXArticulationJointBase()
{
}

bool PhysXArticulationJointBase::InitChip()
{
	if (!Chip::InitChip())
		return false;
	UpdateConnections();
	return true;
}

bool PhysXArticulationJointBase::CopyChip(Chip *chip)
{
	PhysXArticulationJointBase *c = dynamic_cast<PhysXArticulationJointBase*>(chip);
	B_RETURN(Chip::CopyChip(c));
	_globalFrame = c->_globalFrame;
	UpdateConnections();
	return true;
}

bool PhysXArticulationJointBase::LoadChip(DocumentLoader &loader)
{
	B_RETURN(Chip::LoadChip(loader));
	LOADDEF("globalFrame", _globalFrame, true);
	UpdateConnections();
	return true;
}

bool PhysXArticulationJointBase::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(Chip::SaveChip(saver));
	SAVEDEF("globalFrame", _globalFrame, true);
	return true;
}


void PhysXArticulationJointBase::UpdateConnections()
{
	if (typeid(*this) == typeid(PhysXArticulationJointBase))
		return; // Skip child connections if not a subtype of Joint!

//	CREATE_CHILD_KEEP(0, PHYSXRIGIDACTOR_GUID, false, BOTH, L"Actor 1");
//	CREATE_CHILD_KEEP(1, PHYSXRIGIDACTOR_GUID, false, BOTH, L"Actor 2 (Optional)");

	if (_globalFrame) {
		CREATE_CHILD_KEEP(1, MATRIXCHIP_GUID, false, UP, L"Global Frame");
		ClearConnections(2);
	}
	else {
		CREATE_CHILD_KEEP(1, MATRIXCHIP_GUID, false, UP, L"Parent Frame");
		CREATE_CHILD_KEEP(2, MATRIXCHIP_GUID, false, UP, L"Child Frame");
		ClearConnections(3);
	}
}

void PhysXArticulationJointBase::SetGlobalFrame(bool gf) 
{
	_globalFrame = gf; 
	UpdateConnections();
}
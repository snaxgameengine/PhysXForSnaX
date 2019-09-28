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
#include "PhysXArticulationRC.h"
#include "PhysXScene.h"
#include "PhysXArticulationLink.h"
#include "PhysXArticulationJointBase.h"

using namespace m3d;


CHIPDESCV1_DEF(PhysXArticulationRC, L"PhysX Articulation RC", PHYSXARTICULATIONRC_GUID, PHYSXARTICULATIONBASE_GUID);



PhysXArticulationRC::PhysXArticulationRC()
{
	_articulation = nullptr;
}

PhysXArticulationRC::~PhysXArticulationRC()
{
}

bool PhysXArticulationRC::CopyChip(Chip *chip)
{
	PhysXArticulationRC *c = dynamic_cast<PhysXArticulationRC*>(chip);
	B_RETURN(PhysXArticulationBase::CopyChip(c));
	_flags = c->_flags;
	return true;
}

bool PhysXArticulationRC::LoadChip(DocumentLoader &loader)
{
	B_RETURN(PhysXArticulationBase::LoadChip(loader));
	LOADDEF("flags", (PxArticulationFlags::InternalType&)_flags, 0);
	return true;
}

bool PhysXArticulationRC::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(PhysXArticulationBase::SaveChip(saver));
	SAVEDEF("flags", (const PxArticulationFlags::InternalType&)_flags, 0);
	return true;
}

void PhysXArticulationRC::CallChip()
{
	if (Exist())
		return;

	_articulation = (PxArticulationReducedCoordinate*)CreateArticulation();

	if (_articulation)
		((PhysXScene*)_articulation->getScene()->userData)->RegisterSceneObject(this);
}

PxArticulationBase *PhysXArticulationRC::CreateArticulation()
{
	ChildPtr<PhysXScene> chScene = GetChild(0);
	if (!chScene)
		return nullptr;

	PxScene *scene = chScene->GetScene();
	if (!scene)
		return nullptr;

	ChildPtr<PhysXArticulationLink> chLink = GetChild(1);
	if (!chLink)
		return nullptr;

	PxArticulationReducedCoordinate *arti = scene->getPhysics().createArticulationReducedCoordinate();
	if (!arti)
		return nullptr;

	arti->setArticulationFlags(_flags);
	arti->setSolverIterationCounts(_minPositionIters, _minVelocityIters);
	arti->setSleepThreshold(_sleepThreshold);

	PxArticulationLink *rootLink = chLink->CreateLink(arti, nullptr);
	if (!rootLink) {
		arti->release();
		return nullptr;
	}

	scene->addArticulation(*arti);

	return arti;
}

void PhysXArticulationRC::DestroyArticulation() 
{
	if (!_articulation)
		return;

	if (_articulation->getScene())
		((PhysXScene*)_articulation->getScene()->userData)->UnregisterSceneObject(this);
	List<PxArticulationLink*> links(_articulation->getNbLinks(), (PxArticulationLink*)nullptr);
	_articulation->getLinks(&links.front(), _articulation->getNbLinks());
	for (size_t i = 0; i < links.size(); i++) {
		PxArticulationLink *l = links[i];
		const PhysXActorData *d =(const PhysXActorData*)l->userData;
		if (d) {
			PhysXArticulationLink *chip = dynamic_cast<PhysXArticulationLink*>(d->chip);
			if (chip)
				chip->ReleaseLink();
			PhysXArticulationJointBase *linkChip = dynamic_cast<PhysXArticulationJointBase*>(d->linkChip);
			if (linkChip)
				linkChip->ReleaseJoint();
			mmdelete(d);
		}
	}
	_articulation->release();
	_articulation = nullptr;
}

PxArticulationFlags PhysXArticulationRC::GetFlags() const
{
	return _flags;
}

void PhysXArticulationRC::SetFlags(PxArticulationFlags f)
{
	_flags = f;

	if (_articulation)
		_articulation->setArticulationFlags(f);
}
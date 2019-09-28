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
#pragma once

#include "Exports.h"
#include "PhysXRigidBody.h"


namespace m3d
{


static const GUID PHYSXARTICULATIONLINK_GUID = { 0x451faab0, 0x5751, 0x4694,{ 0x85, 0xb6, 0x1d, 0xd2, 0x6a, 0x13, 0xc0, 0x14 } };



class PHYSXCHIPS_API PhysXArticulationLink : public PhysXRigidBody
{
	CHIPDESC_DECL;
public:
	PhysXArticulationLink();
	~PhysXArticulationLink();

	PxRigidBody *GetRigidBody() const override { return _link; }

	virtual PxArticulationLink *CreateLink(PxArticulationReducedCoordinate *a, PxArticulationLink *p);
	virtual void ReleaseLink();

	PxActor *GetActor() const override { return _link; }
	void DestroyActor() {} // N/A

protected:
	PxArticulationLink *_link;
};



}
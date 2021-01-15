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
#include "M3DEngine/Chip.h"
#include <extensions\PxExtensionsAPI.h>
//#include <task\PxTask.h>

using namespace physx;

namespace m3d
{


static const GUID PHYSXSDK_GUID = { 0x4a56e38e, 0xe6d0, 0x4e25, { 0x95, 0x7c, 0x85, 0x47, 0xe9, 0xa3, 0x33, 0x80 } };

#define PX_RELEASE(x) if (x) { x->release(); x = nullptr; }


class PHYSXCHIPS_API PhysXSDK : public Chip
{
	CHIPDESC_DECL;
public:
	PhysXSDK();
	virtual ~PhysXSDK();

	virtual PxPhysics *GetPhysics() const { return _physics; }
	virtual PxCooking *GetCooking();
	virtual PxCudaContextManager *GetCudaContextManager();

	virtual PxMaterial *GetDefaultMaterial();

	virtual const PxTolerancesScale &GetToleranceScale() const { return _toleranceScale; }

protected:
	PxTolerancesScale _toleranceScale;

	PxFoundation *_foundation;
//	PxProfileZoneManager *_pzm;
	PxPhysics *_physics;
	bool _extensionsOK;
	PxMaterial *_defaultMaterial;
	PxCooking *_cooking;
	PxCudaContextManager *_cudaContextManager;

};

class PHYSXCHIPS_API PhysXUsage
{
public:
	PhysXUsage();
	virtual ~PhysXUsage();

	PhysXSDK *sdk() const;
	PxPhysics *physics() const;

private:
	mutable PhysXSDK *_sdk;
};



}
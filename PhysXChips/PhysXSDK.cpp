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
#include "PhysXSDK.h"


using namespace m3d;


CHIPDESCV1_DEF_HIDDEN(PhysXSDK, L"PhysX SDK", PHYSXSDK_GUID, CHIP_GUID);


class Allocer : public PxAllocatorCallback
{
        void* allocate(size_t size, const char *file, const char*, int line)
        {
#if defined(DEBUG) || defined(_DEBUG)
            return mm().getDebug(size, file, line);// mmalloc(size);
#else
            return mm().get(size);
#endif
        }

        void deallocate(void* ptr)
        {
#if defined(DEBUG) || defined(_DEBUG)
			mm().freeDebug(ptr);
#else
			mm().free(ptr);
#endif
        }
};
//static Allocer gDefaultAllocatorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;  // Note using default allocator to avoid 16-byte alignement issues for x86. TODO: fix it!

class Errors : public PxErrorCallback
{
public:
        virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line)
		{
			MessageSeverity sev = INFO;
			const char* errorCode = NULL;

			switch (code)
			{
			case PxErrorCode::eNO_ERROR:
				sev = INFO;
				errorCode = "No Error";
			case PxErrorCode::eDEBUG_INFO:
				sev = DINFO;
				errorCode = "Info";
				break;
			case PxErrorCode::eDEBUG_WARNING:
				sev = WARN;
				errorCode = "Warning";
				break;
			case PxErrorCode::eINVALID_PARAMETER:
				sev = FATAL;
				errorCode = "Invalid Parameter";
				break;
			case PxErrorCode::eINVALID_OPERATION:
				sev = FATAL;
				errorCode = "Invalid Operation";
				break;
			case PxErrorCode::eOUT_OF_MEMORY:
				sev = FATAL;
				errorCode = "Out of Memory";
				break;
			case PxErrorCode::eINTERNAL_ERROR:
				sev = FATAL;
				errorCode = "Internal Error";
				break;
			case PxErrorCode::eABORT:
				sev = FATAL;
				errorCode = "Abort";
				break;
			case PxErrorCode::ePERF_WARNING:
				sev = NOTICE;
				errorCode = "Performance Warning";
				break;
			default:
				errorCode = "Unknown Error";
				break;
			}

			msg(sev, String::fromStringA(StringA::format("%s: %s  [%s (%d)]", errorCode, message, file, line)));
	}
};
static Errors gDefaultErrorCallback;

PhysXSDK::PhysXSDK()
{
#if defined(DEBUG) || defined(_DEBUG)
	bool recordMemoryAllocations = true;
#else
	bool recordMemoryAllocations = false;
#endif

	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);

//	_pzm = &PxProfileZoneManager::createProfileZoneManager(_foundation);
	
	_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, _toleranceScale, recordMemoryAllocations, nullptr);

	_extensionsOK = PxInitExtensions(*_physics, nullptr);

	_defaultMaterial = nullptr;
	_cooking = nullptr;
	_cudaContextManager = nullptr;
}

PhysXSDK::~PhysXSDK()
{
	PX_RELEASE(_cudaContextManager);
	PX_RELEASE(_cooking);
	PX_RELEASE(_defaultMaterial);

	if (_extensionsOK)
		PxCloseExtensions();

	PX_RELEASE(_physics);

//	if (_pzm)
//		_pzm->release();
	PX_RELEASE(_foundation);
}

PxMaterial *PhysXSDK::GetDefaultMaterial()
{
	if (!_defaultMaterial)
		_defaultMaterial = _physics->createMaterial(0.5f, 0.5f, 0.1f);

	return _defaultMaterial;
}

PxCooking *PhysXSDK::GetCooking()
{
	if (!_cooking)
		_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *_foundation, PxCookingParams(_toleranceScale));
	return _cooking;
}

PxCudaContextManager *PhysXSDK::GetCudaContextManager()
{
	if (_cudaContextManager && _cudaContextManager->contextIsValid())
		return _cudaContextManager;

//	int gg =PxGetSuggestedCudaDeviceOrdinal(gDefaultErrorCallback);

	PxCudaContextManagerDesc cudaContextManagerDesc;
//	cudaContextManagerDesc.graphicsDevice = ((D3DGraphics*)engine->GetGraphics())->GetDevice();
//	cudaContextManagerDesc.interopMode = pxtask::CudaInteropMode::D3D11_INTEROP;
	_cudaContextManager = PxCreateCudaContextManager(*_foundation, cudaContextManagerDesc);
	if (!_cudaContextManager)
		return nullptr;

	return _cudaContextManager->contextIsValid() ? _cudaContextManager : nullptr;
}


PhysXUsage::PhysXUsage() : _sdk(nullptr)
{
}

PhysXUsage::~PhysXUsage()
{
}


PhysXSDK *PhysXUsage::sdk() const
{
	if (!_sdk)
		_sdk = (PhysXSDK*)engine->GetChipManager()->GetGlobalChip(PHYSXSDK_GUID);
	return _sdk;
}

PxPhysics *PhysXUsage::physics() const
{
	PhysXSDK *p = sdk();
	if (p)
		return p->GetPhysics();
	return nullptr;
}
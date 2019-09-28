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
#include "Exports.h"
#include "M3DEngine/ChipDef.h"
#include "M3DEngine/ProjectDependencies.h"

using namespace m3d;


namespace 
{
	const unsigned MAX_CHIPS = 128;
	ChipDesc chipDescs[128];
	unsigned chipCount = 0;
}

// Defined in ChipDesc.h. 
const ChipDesc &m3d::RegisterChipDesc(const wchar_t *name, GUID type, GUID basetype, ChipDesc::Usage usage, unsigned version, const char *factoryFunc, const wchar_t *filters)
{
	ChipDesc cd = { name, type, basetype, usage, version, factoryFunc, filters };
	assert(chipCount < MAX_CHIPS);
	return chipDescs[chipCount++] = cd;
}


extern "C" 
{
	__declspec( dllexport ) const ChipDesc& __cdecl GetChipDesc(unsigned index) { return chipDescs[index]; }
	__declspec( dllexport ) unsigned __cdecl GetChipCount() { return chipCount; } 
	__declspec( dllexport ) const wchar_t* __cdecl GetPacketName() { return L"PhysX"; }
	__declspec( dllexport ) bool __cdecl OnPacketQuery() { return true; }
	__declspec( dllexport ) bool __cdecl OnPacketLoad() { return true; }
	__declspec( dllexport ) void __cdecl OnPacketUnload() { }
	__declspec( dllexport ) void __cdecl AddDependencies(ProjectDependencies &deps) 
	{
		if (deps.GetPlatform() == ProjectDependencies::WD_X64) 
		{
			deps.AddDependency(L"PhysX_64.dll");
			deps.AddDependency(L"PhysXCommon_64.dll");
			deps.AddDependency(L"PhysXCooking_64.dll");	
			deps.AddDependency(L"PhysXFoundation_64.dll");
			deps.AddDependency(L"PhysXGPU_64.dll");
		}
	}
	__declspec( dllexport ) unsigned __cdecl GetSupportedPlatforms() { return PLATFORM_WINDESKTOP_X64; }
}


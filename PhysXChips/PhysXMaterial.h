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
#include "PhysXSDK.h"


namespace m3d
{


static const GUID PHYSXMATERIAL_GUID = { 0xfd80fcef, 0x652f, 0x4ac7, { 0xbe, 0x54, 0x18, 0x0d, 0xc4, 0xfa, 0xcd, 0x2e } };



class PHYSXCHIPS_API PhysXMaterial : public Chip, public PhysXUsage
{
	CHIPDESC_DECL;
public:
	PhysXMaterial();
	virtual ~PhysXMaterial();

	virtual bool CopyChip(Chip *chip) override;
	virtual bool LoadChip(DocumentLoader &loader) override;
	virtual bool SaveChip(DocumentSaver &saver) const override;

	virtual PxMaterial *GetMaterial();
	virtual PxMaterial *GetMaterialIfExist() const { return _material; }

	virtual float GetStaticFriction() const;
	virtual float GetDynamicFriction() const;
	virtual float GetRestitution() const;
	virtual bool IsDisableFriction() const;
	virtual bool IsDisableStrongFriction() const;
	virtual PxCombineMode::Enum GetFrictionCombineMode() const;
	virtual PxCombineMode::Enum GetRestitutionCombineMode() const;

	virtual void SetStaticFriction(float v, bool chipSettings = true);
	virtual void SetDynamicFriction(float v, bool chipSettings = true);
	virtual void SetRestitution(float v, bool chipSettings = true);
	virtual void SetDisableFriction(bool v, bool chipSettings = true);
	virtual void SetDisableStrongFriction(bool v, bool chipSettings = true);
	virtual void SetFrictionCombineMode(PxCombineMode::Enum v, bool chipSettings = true);
	virtual void SetRestitutionCombineMode(PxCombineMode::Enum v, bool chipSettings = true);

protected:
	PxMaterial *_material;

	float _staticFriction;
	float _dynamicFriction;
	float _restitution;
	bool _disableFriction;
	bool _disableStrongFriction;
	PxCombineMode::Enum _frictionCombineMode;
	PxCombineMode::Enum _resitutionCombineMode;
};



}
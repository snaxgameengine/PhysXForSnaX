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
#include "PhysXMaterial.h"
#include "PhysXSDK.h"

using namespace m3d;


CHIPDESCV1_DEF(PhysXMaterial, L"PhysX Material", PHYSXMATERIAL_GUID, CHIP_GUID);


PhysXMaterial::PhysXMaterial()
{
	_material = nullptr;
	_staticFriction = 0.5f;
	_dynamicFriction = 0.5f;
	_restitution = 0.1f;
	_disableFriction = false;
	_disableStrongFriction = false;
	_frictionCombineMode = PxCombineMode::eAVERAGE;
	_resitutionCombineMode = PxCombineMode::eAVERAGE;
}

PhysXMaterial::~PhysXMaterial()
{
	PX_RELEASE(_material)
}

bool PhysXMaterial::CopyChip(Chip *chip)
{
	PhysXMaterial *c = dynamic_cast<PhysXMaterial*>(chip);
	B_RETURN(Chip::CopyChip(c));
	_staticFriction = c->_staticFriction;
	_dynamicFriction = c->_dynamicFriction;
	_restitution = c->_restitution;
	_disableFriction = c->_disableFriction;
	_disableStrongFriction = c->_disableStrongFriction;
	_frictionCombineMode = c->_frictionCombineMode;
	_resitutionCombineMode = c->_resitutionCombineMode;
	PX_RELEASE(_material)
	return true;
}

bool PhysXMaterial::LoadChip(DocumentLoader &loader)
{
	B_RETURN(Chip::LoadChip(loader));
	LOADDEF("staticFriction", _staticFriction, 0.5f);
	LOADDEF("dynamicFriction", _dynamicFriction, 0.5f);
	LOADDEF("restitution", _restitution, 0.1f);
	LOADDEF("disableFriction", _disableFriction, false);
	LOADDEF("disableStrongFriction", _disableStrongFriction, false);
	LOADDEF("frictionCombineMode", (unsigned&)_frictionCombineMode, PxCombineMode::eAVERAGE);
	LOADDEF("resitutionCombineMode", (unsigned&)_resitutionCombineMode, PxCombineMode::eAVERAGE);
	PX_RELEASE(_material)
	return true;
}

bool PhysXMaterial::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(Chip::SaveChip(saver));
	SAVEDEF("staticFriction", _staticFriction, 0.5f);
	SAVEDEF("dynamicFriction", _dynamicFriction, 0.5f);
	SAVEDEF("restitution", _restitution, 0.1f);
	SAVEDEF("disableFriction", _disableFriction, false);
	SAVEDEF("disableStrongFriction", _disableStrongFriction, false);
	SAVEDEF("frictionCombineMode", (unsigned)_frictionCombineMode, PxCombineMode::eAVERAGE);
	SAVEDEF("resitutionCombineMode", (unsigned)_resitutionCombineMode, PxCombineMode::eAVERAGE);
	return true;
}

PxMaterial *PhysXMaterial::GetMaterial()
{
	if (_material)
		return _material;

	_material = physics()->createMaterial(_staticFriction, _dynamicFriction, _restitution);
	_material->setFlag(PxMaterialFlag::eDISABLE_FRICTION, _disableFriction);
	_material->setFlag(PxMaterialFlag::eDISABLE_STRONG_FRICTION, _disableStrongFriction);
	_material->setFrictionCombineMode(_frictionCombineMode);
	_material->setRestitutionCombineMode(_resitutionCombineMode);
	return _material;
}

float PhysXMaterial::GetStaticFriction() const
{
	PxMaterial *m = GetMaterialIfExist();
	if (m)
		return m->getStaticFriction();
	return _staticFriction;
}

float PhysXMaterial::GetDynamicFriction() const
{
	PxMaterial *m = GetMaterialIfExist();
	if (m)
		return m->getDynamicFriction();
	return _dynamicFriction;
}

float PhysXMaterial::GetRestitution() const
{
	PxMaterial *m = GetMaterialIfExist();
	if (m)
		return m->getRestitution();
	return _restitution;
}

bool PhysXMaterial::IsDisableFriction() const
{
	PxMaterial *m = GetMaterialIfExist();
	if (m)
		return m->getFlags() == PxMaterialFlag::eDISABLE_FRICTION;
	return _disableFriction;
}

bool PhysXMaterial::IsDisableStrongFriction() const
{
	PxMaterial *m = GetMaterialIfExist();
	if (m)
		return m->getFlags() == PxMaterialFlag::eDISABLE_STRONG_FRICTION;
	return _disableStrongFriction;
}

PxCombineMode::Enum PhysXMaterial::GetFrictionCombineMode() const
{
	PxMaterial *m = GetMaterialIfExist();
	if (m)
		return m->getFrictionCombineMode();
	return _frictionCombineMode;
}

PxCombineMode::Enum PhysXMaterial::GetRestitutionCombineMode() const
{
	PxMaterial *m = GetMaterialIfExist();
	if (m)
		return m->getRestitutionCombineMode();
	return _resitutionCombineMode;
}

void PhysXMaterial::SetStaticFriction(float v, bool chipSettings)
{
	PxMaterial *m = GetMaterialIfExist();
	if (m)
		m->setStaticFriction(v);
	if (chipSettings)
		_staticFriction = v;
}

void PhysXMaterial::SetDynamicFriction(float v, bool chipSettings)
{
	PxMaterial *m = GetMaterialIfExist();
	if (m)
		m->setDynamicFriction(v);
	if (chipSettings)
		_dynamicFriction = v;
}

void PhysXMaterial::SetRestitution(float v, bool chipSettings)
{
	PxMaterial *m = GetMaterialIfExist();
	if (m)
		m->setRestitution(v);
	if (chipSettings)
		_restitution = v;
}

void PhysXMaterial::SetDisableFriction(bool v, bool chipSettings)
{
	PxMaterial *m = GetMaterialIfExist();
	if (m)
		m->setFlag(PxMaterialFlag::eDISABLE_FRICTION, v);
	if (chipSettings)
		_disableFriction = v;
}

void PhysXMaterial::SetDisableStrongFriction(bool v, bool chipSettings)
{
	PxMaterial *m = GetMaterialIfExist();
	if (m)
		m->setFlag(PxMaterialFlag::eDISABLE_STRONG_FRICTION, v);
	if (chipSettings)
		_disableStrongFriction = v;
}

void PhysXMaterial::SetFrictionCombineMode(PxCombineMode::Enum v, bool chipSettings)
{
	PxMaterial *m = GetMaterialIfExist();
	if (m)
		m->setFrictionCombineMode(v);
	if (chipSettings)
		_frictionCombineMode = v;
}

void PhysXMaterial::SetRestitutionCombineMode(PxCombineMode::Enum v, bool chipSettings)
{
	PxMaterial *m = GetMaterialIfExist();
	if (m)
		m->setRestitutionCombineMode(v);
	if (chipSettings)
		_resitutionCombineMode = v;
}

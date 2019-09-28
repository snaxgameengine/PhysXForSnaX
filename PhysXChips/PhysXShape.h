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

namespace m3d
{

	
static const GUID PHYSXSHAPE_GUID = { 0x14842055, 0x49ee, 0x47e1, { 0x86, 0xe2, 0xf2, 0x32, 0xeb, 0xd9, 0x0b, 0x02 } };



class PHYSXCHIPS_API PhysXShape : public Chip
{
	CHIPDESC_DECL;
public:
	PhysXShape();
	virtual ~PhysXShape();

	virtual bool CopyChip(Chip *chip) override;
	virtual bool LoadChip(DocumentLoader &loader) override;
	virtual bool SaveChip(DocumentSaver &saver) const override;

	enum Type { T_NONE, T_BOX, T_CAPSULE, T_CONVEX_MESH, T_HEIGHT_FIELD, T_PLANE, T_SPHERE, T_TRIANGLE_MESH };

	virtual Type GetType() const { return _type; }
	virtual void SetType(Type t);

	virtual PxShape *GetShape();
	virtual PxReal GetMass() const { return _mass; }

protected:
	PxShape *_shape;
	PxReal _mass;

	Type _type;
	float _contactOffset;
	float _restOffset;

/*
  eSIMULATION_SHAPE = (1<<0), 
  eSCENE_QUERY_SHAPE = (1<<1), 
  eTRIGGER_SHAPE = (1<<2), 
  eVISUALIZATION = (1<<3), 
  ePARTICLE_DRAIN = (1<<4), 
  eUSE_SWEPT_BOUNDS = (1<<6) 
*/
};



}
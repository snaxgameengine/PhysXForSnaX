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
#include "StdChips/Vector.h"

namespace m3d
{


static const GUID PHYSXVECTOR_GUID = { 0xdb013520, 0x3b43, 0x4520, { 0xa6, 0xf8, 0x2d, 0x16, 0xaa, 0x20, 0xd1, 0x72 } };



class PHYSXCHIPS_API PhysXVector : public Vector
{
	CHIPDESC_DECL;
public:
	PhysXVector();
	virtual ~PhysXVector();

	virtual bool CopyChip(Chip *chip) override;
	virtual bool LoadChip(DocumentLoader &loader) override;
	virtual bool SaveChip(DocumentSaver &saver) const override;

	virtual const XMFLOAT4 &GetVector() override;
	virtual void SetVector(const XMFLOAT4 &v) override;

	enum Command { CMD_NONE, 
		CMD_SCENE_XET_GRAVITY,
		CMD_RIGID_BODY_XET_LINEAR_VELOCITY = 100,
		CMD_RIGID_BODY_XET_ANGULAR_VELOCITY,
		CMD_RIGID_BODY_XET_MASS_SPACE_INERTIA_TENSOR,
		CMD_CONSTRAINT_GET_LINEAR_FORCE = 200,
		CMD_CONSTRAINT_GET_ANGULAR_FORCE
	};

	virtual Command GetCommand() const { return _cmd; }
	virtual void SetCommand(Command cmd);

protected:
	Command _cmd;

};

}
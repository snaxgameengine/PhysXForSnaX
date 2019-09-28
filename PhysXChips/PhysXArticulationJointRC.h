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
#include "PhysXArticulationJointBase.h"


namespace m3d
{


static const GUID PHYSXARTICULATIONJOINTRC_GUID = { 0xdb90b177, 0x3336, 0x48b5,{ 0x8c, 0xb, 0xec, 0xc8, 0x18, 0xee, 0xa2, 0x94 } };



class PHYSXCHIPS_API PhysXArticulationJointRC : public PhysXArticulationJointBase
{
	CHIPDESC_DECL;
public:
	PhysXArticulationJointRC();
	virtual ~PhysXArticulationJointRC();

	bool CopyChip(Chip *chip) override;
	bool LoadChip(DocumentLoader &loader) override;
	bool SaveChip(DocumentSaver &saver) const override;

	virtual PxArticulationLink *CreateLink(PxArticulationReducedCoordinate *a, PxArticulationLink *p);

	void ReleaseJoint() override;

	struct AxisParams
	{
		PxArticulationMotion::Enum motion;
		PxReal lowLimit;
		PxReal highLimit;

		PxReal stiffness;
		PxReal damping;
		PxReal maxForce;
		bool isAccelerationDrive;

		PxReal driveTarget;
		PxReal driveTargetVel;

		AxisParams() : motion(PxArticulationMotion::eLOCKED), lowLimit(0.0f), highLimit(0.0f), stiffness(0.0f), damping(0.0f), maxForce(FLT_MAX), isAccelerationDrive(false), driveTarget(0.0f), driveTargetVel(0.0f) {}

		bool operator!=(const AxisParams &rhs) const
		{
			return motion != rhs.motion || lowLimit != rhs.lowLimit || highLimit != rhs.highLimit || stiffness != rhs.stiffness || damping != rhs.damping || maxForce != rhs.maxForce || isAccelerationDrive != rhs.isAccelerationDrive || driveTarget != rhs.driveTarget || driveTargetVel != rhs.driveTargetVel; 
		}
	};

	virtual PxArticulationJointType::Enum GetType() const { return _jointType; }
	virtual PxReal GetFrictionCoefficient() const { return _frictionCoefficient; }
	virtual PxReal GetMaxJointVelocity() const { return _maxJointVelocity; }
	virtual const AxisParams &GetAxisParams(PxArticulationAxis::Enum axis) const { return _axisParams[axis]; }

	virtual void SetType(PxArticulationJointType::Enum t);
	virtual void SetFrictionCoefficient(PxReal f);
	virtual void SetMaxJointVelocity(PxReal f);
	virtual void SetMotion(PxArticulationAxis::Enum axis, PxArticulationMotion::Enum motion);
	virtual void SetLowLimit(PxArticulationAxis::Enum axis, PxReal lowLimit);
	virtual void SetHighLimit(PxArticulationAxis::Enum axis, PxReal highLimit);
	virtual void SetStiffness(PxArticulationAxis::Enum axis, PxReal stiffness);
	virtual void SetDamping(PxArticulationAxis::Enum axis, PxReal damping);
	virtual void SetMaxForce(PxArticulationAxis::Enum axis, PxReal maxForce);
	virtual void SetIsAccelerationDrive(PxArticulationAxis::Enum axis, bool isAccelerationDrive);
	virtual void SetDriveTarget(PxArticulationAxis::Enum axis, PxReal driveTarget);
	virtual void SetDriveTargetVel(PxArticulationAxis::Enum axis, PxReal driveTargetVel);


protected:
	PxArticulationJointReducedCoordinate *_joint;
	
	PxArticulationJointType::Enum _jointType;
	PxReal _frictionCoefficient;
	PxReal _maxJointVelocity;
	AxisParams _axisParams[PxArticulationAxis::eCOUNT];

};



}

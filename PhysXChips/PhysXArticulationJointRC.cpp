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
#include "PhysXArticulationJointRC.h"
#include "PhysXArticulationLink.h"
#include "StdChips/MatrixChip.h"

using namespace m3d;


CHIPDESCV1_DEF(PhysXArticulationJointRC, L"PhysX Articulation Joint RC", PHYSXARTICULATIONJOINTRC_GUID, PHYSXARTICULATIONJOINTBASE_GUID);


namespace m3d
{
	bool SerializeDocumentData(DocumentSaver &saver, const PhysXArticulationJointRC::AxisParams &data)
	{
		SAVEDEF("motion", data.motion, PxArticulationMotion::eLOCKED);
		SAVEDEF("lowLimit", data.lowLimit, 0.0f);
		SAVEDEF("highLimit", data.highLimit, 0.0f);
		SAVEDEF("stiffness", data.stiffness, 0.0f);
		SAVEDEF("damping", data.damping, 0.0f);
		SAVEDEF("maxForce", data.maxForce, FLT_MAX);
		SAVEDEF("isAccelerationDrive", data.isAccelerationDrive, false);
		SAVEDEF("driveTarget", data.driveTarget, 0.0f);
		SAVEDEF("driveTargetVel", data.driveTargetVel, 0.0f);
		return true;
	}

	bool DeserializeDocumentData(DocumentLoader &loader, PhysXArticulationJointRC::AxisParams &data)
	{
		LOADDEF("motion", data.motion, PxArticulationMotion::eLOCKED);
		LOADDEF("lowLimit", data.lowLimit, 0.0f);
		LOADDEF("highLimit", data.highLimit, 0.0f);
		LOADDEF("stiffness", data.stiffness, 0.0f);
		LOADDEF("damping", data.damping, 0.0f);
		LOADDEF("maxForce", data.maxForce, FLT_MAX);
		LOADDEF("isAccelerationDrive", data.isAccelerationDrive, false);
		LOADDEF("driveTarget", data.driveTarget, 0.0f);
		LOADDEF("driveTargetVel", data.driveTargetVel, 0.0f);
		return true;
	}
}


PhysXArticulationJointRC::PhysXArticulationJointRC()
{
	_joint = nullptr;
	_jointType = PxArticulationJointType::eFIX;
	_frictionCoefficient = 0.0f;
	_maxJointVelocity = FLT_MAX;
}

PhysXArticulationJointRC::~PhysXArticulationJointRC()
{
	if (_joint) {
		PhysXActorData *d = (PhysXActorData*)_joint->getChildArticulationLink().userData;
		if (d && d->linkChip == this)
			d->linkChip = nullptr;
	}
}

bool PhysXArticulationJointRC::CopyChip(Chip *chip)
{
	PhysXArticulationJointRC *c = dynamic_cast<PhysXArticulationJointRC*>(chip);
	B_RETURN(PhysXArticulationJointBase::CopyChip(c));
	_jointType = c->_jointType;
	_frictionCoefficient = c->_frictionCoefficient;
	_maxJointVelocity = c->_maxJointVelocity;
	memcpy(_axisParams, c->_axisParams, sizeof(_axisParams));
	return true;
}

bool PhysXArticulationJointRC::LoadChip(DocumentLoader &loader)
{
	B_RETURN(PhysXArticulationJointBase::LoadChip(loader));
	LOADDEF("jointType", (unsigned&)_jointType, PxArticulationJointType::eFIX);
	LOADDEF("FrictionCoefficient", _frictionCoefficient, 0.0f);
	LOADDEF("MaxJointVelocity", _maxJointVelocity, 0.0f);
	LOADDEF("axisTwist", _axisParams[PxArticulationAxis::eTWIST], AxisParams());
	LOADDEF("axisSwing1", _axisParams[PxArticulationAxis::eSWING1], AxisParams());
	LOADDEF("axisSwing2", _axisParams[PxArticulationAxis::eSWING2], AxisParams());
	LOADDEF("axisX", _axisParams[PxArticulationAxis::eX], AxisParams());
	LOADDEF("axisY", _axisParams[PxArticulationAxis::eY], AxisParams());
	LOADDEF("axisZ", _axisParams[PxArticulationAxis::eZ], AxisParams());
	return true;
}

bool PhysXArticulationJointRC::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(PhysXArticulationJointBase::SaveChip(saver));
	SAVEDEF("jointType", (const unsigned&)_jointType, PxArticulationJointType::eFIX);
	SAVEDEF("frictionCoefficient", _frictionCoefficient, 0.0f);
	SAVEDEF("maxJointVelocity", _maxJointVelocity, 0.0f);
	SAVEDEF("axisTwist", _axisParams[PxArticulationAxis::eTWIST], AxisParams());
	SAVEDEF("axisSwing1", _axisParams[PxArticulationAxis::eSWING1], AxisParams());
	SAVEDEF("axisSwing2", _axisParams[PxArticulationAxis::eSWING2], AxisParams());
	SAVEDEF("axisX", _axisParams[PxArticulationAxis::eX], AxisParams());
	SAVEDEF("axisY", _axisParams[PxArticulationAxis::eY], AxisParams());
	SAVEDEF("axisZ", _axisParams[PxArticulationAxis::eZ], AxisParams());
	return true;

}

PxArticulationLink *PhysXArticulationJointRC::CreateLink(PxArticulationReducedCoordinate *a, PxArticulationLink *p)
{
	if (_joint)
		return nullptr;

	if (!a || !p)
		return nullptr;
	ChildPtr<PhysXArticulationLink> ch0 = GetChild(0);
	if (!ch0)
		return nullptr;
	PxArticulationLink *l = ch0->CreateLink(a, p);
	if (!l)
		return nullptr;

	PhysXActorData *d = (PhysXActorData*)l->userData;
	if (d)
		d->linkChip = this;

	_joint = static_cast<PxArticulationJointReducedCoordinate*>(l->getInboundJoint());

	PxTransform f1 = PxTransform(PxIdentity);
	PxTransform f2 = PxTransform(PxIdentity);

	if (_globalFrame) {
		ChildPtr<MatrixChip> chGlobalFrame = GetChild(1);
		XMFLOAT3 t;
		XMFLOAT4 r;
		if (chGlobalFrame && chGlobalFrame->GetDecomposedMatrix(t, r, XMFLOAT3())) {
			f1 = p->getGlobalPose().transformInv(PxTransform((PxVec3&)t, (PxQuat&)r));
			f2 = l->getGlobalPose().transformInv(PxTransform((PxVec3&)t, (PxQuat&)r));
		}
	}
	else {
		ChildPtr<MatrixChip> chFrame1 = GetChild(1);
		ChildPtr<MatrixChip> chFrame2 = GetChild(2);
		XMFLOAT3 t;
		XMFLOAT4 r;
		if (chFrame1 && chFrame1->GetDecomposedMatrix(t, r, XMFLOAT3()))
			f1 = PxTransform((PxVec3&)t, (PxQuat&)r);
		if (chFrame2 && chFrame2->GetDecomposedMatrix(t, r, XMFLOAT3()))
			f2 = PxTransform((PxVec3&)t, (PxQuat&)r);
	}
	
	_joint->setParentPose(f1);
	_joint->setChildPose(f2);
	_joint->setJointType(_jointType);
	_joint->setFrictionCoefficient(_frictionCoefficient);
	_joint->setMaxJointVelocity(_maxJointVelocity);
	for (unsigned i = 0; i < PxArticulationAxis::eCOUNT; i++) {
		if (_axisParams[PxArticulationAxis::eTWIST + i].motion != PxArticulationMotion::eLOCKED) {
			auto a = (PxArticulationAxis::Enum)(PxArticulationAxis::eTWIST + i);
			_joint->setMotion(a, _axisParams[a].motion);
			_joint->setLimit(a, _axisParams[a].lowLimit, _axisParams[a].highLimit);
			_joint->setDrive(a, _axisParams[a].stiffness, _axisParams[a].damping, _axisParams[a].maxForce, _axisParams[a].isAccelerationDrive ? PxArticulationDriveType::eACCELERATION : PxArticulationDriveType::eFORCE);
			_joint->setDriveTarget(a, _axisParams[a].driveTarget);
			_joint->setDriveVelocity(a, _axisParams[a].driveTargetVel);
		}
	}

	return l;
}

void PhysXArticulationJointRC::ReleaseJoint()
{
	_joint = nullptr;
}

void PhysXArticulationJointRC::SetType(PxArticulationJointType::Enum t)
{
	_jointType = t;
	if (_joint) {
		_joint->getChildArticulationLink().getArticulation().wakeUp();
		_joint->setJointType(t);
	}
}

void PhysXArticulationJointRC::SetFrictionCoefficient(PxReal f)
{
	_frictionCoefficient = f;
	if (_joint) {
		_joint->getChildArticulationLink().getArticulation().wakeUp();
		_joint->setFrictionCoefficient(f);
	}
}

void PhysXArticulationJointRC::SetMaxJointVelocity(PxReal f)
{
	_maxJointVelocity = f;
	if (_joint) {
		_joint->getChildArticulationLink().getArticulation().wakeUp();
		_joint->setMaxJointVelocity(f);
	}
}

void PhysXArticulationJointRC::SetMotion(PxArticulationAxis::Enum axis, PxArticulationMotion::Enum motion)
{
	_axisParams[axis].motion = motion;
	if (_joint) {
		_joint->getChildArticulationLink().getArticulation().wakeUp();
		_joint->setMotion(axis, motion);
	}
}

void PhysXArticulationJointRC::SetLowLimit(PxArticulationAxis::Enum axis, PxReal lowLimit)
{
	_axisParams[axis].lowLimit = lowLimit;
	if (_joint) {
		_joint->getChildArticulationLink().getArticulation().wakeUp();
		_joint->setLimit(axis, lowLimit, _axisParams[axis].highLimit);
	}
}

void PhysXArticulationJointRC::SetHighLimit(PxArticulationAxis::Enum axis, PxReal highLimit)
{
	_axisParams[axis].highLimit = highLimit;
	if (_joint) {
		_joint->getChildArticulationLink().getArticulation().wakeUp();
		_joint->setLimit(axis, _axisParams[axis].lowLimit, highLimit);
	}
}

void PhysXArticulationJointRC::SetStiffness(PxArticulationAxis::Enum axis, PxReal stiffness)
{
	_axisParams[axis].stiffness = stiffness;
	if (_joint) {
		_joint->getChildArticulationLink().getArticulation().wakeUp();
		_joint->setDrive(axis, stiffness, _axisParams[axis].damping, _axisParams[axis].maxForce, _axisParams[axis].isAccelerationDrive ? PxArticulationDriveType::eACCELERATION : PxArticulationDriveType::eFORCE);
	}
}

void PhysXArticulationJointRC::SetDamping(PxArticulationAxis::Enum axis, PxReal damping)
{
	_axisParams[axis].damping = damping;
	if (_joint) {
		_joint->getChildArticulationLink().getArticulation().wakeUp();
		_joint->setDrive(axis, _axisParams[axis].stiffness, damping, _axisParams[axis].maxForce, _axisParams[axis].isAccelerationDrive ? PxArticulationDriveType::eACCELERATION : PxArticulationDriveType::eFORCE);
	}
}

void PhysXArticulationJointRC::SetMaxForce(PxArticulationAxis::Enum axis, PxReal maxForce)
{
	_axisParams[axis].maxForce = maxForce;
	if (_joint) {
		_joint->getChildArticulationLink().getArticulation().wakeUp();
		_joint->setDrive(axis, _axisParams[axis].stiffness, _axisParams[axis].damping, maxForce, _axisParams[axis].isAccelerationDrive ? PxArticulationDriveType::eACCELERATION : PxArticulationDriveType::eFORCE);
	}
}

void PhysXArticulationJointRC::SetIsAccelerationDrive(PxArticulationAxis::Enum axis, bool isAccelerationDrive)
{
	_axisParams[axis].isAccelerationDrive = isAccelerationDrive;
	if (_joint) {
		_joint->getChildArticulationLink().getArticulation().wakeUp();
		_joint->setDrive(axis, _axisParams[axis].stiffness, _axisParams[axis].damping, _axisParams[axis].maxForce, isAccelerationDrive ? PxArticulationDriveType::eACCELERATION : PxArticulationDriveType::eFORCE);
	}
}

void PhysXArticulationJointRC::SetDriveTarget(PxArticulationAxis::Enum axis, PxReal driveTarget)
{
	_axisParams[axis].driveTarget = driveTarget;
	if (_joint) {
		_joint->getChildArticulationLink().getArticulation().wakeUp();
		_joint->setDriveTarget(axis, driveTarget);
	}
}

void PhysXArticulationJointRC::SetDriveTargetVel(PxArticulationAxis::Enum axis, PxReal driveTargetVel)
{
	_axisParams[axis].driveTargetVel = driveTargetVel;
	if (_joint) {
		_joint->getChildArticulationLink().getArticulation().wakeUp();
		_joint->setDriveVelocity(axis, driveTargetVel);
	}
}
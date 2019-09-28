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
#include "PhysXSDK.h"
#include <thread>

namespace m3d
{


static const GUID PHYSXSCENE_GUID = { 0x7bad6997, 0x690d, 0x48d1, { 0xa4, 0x53, 0x1f, 0x60, 0x30, 0xc3, 0x65, 0x15 } };


class PhysXSceneObject;

class PHYSXCHIPS_API PhysXScene : public Chip
{
	CHIPDESC_DECL;
public:
	PhysXScene();
	virtual ~PhysXScene();

	bool CopyChip(Chip *chip) override;
	bool LoadChip(DocumentLoader &loader) override;
	bool SaveChip(DocumentSaver &saver) const override;

	void CallChip() override;

	virtual bool Exist() const { return _scene != nullptr; }
	virtual bool IsSimulating() const { return _isSimulating; }
	virtual bool IsRunning() const { return _isRunning; }
	virtual PxScene *GetScene();
	virtual void DestroyScene();
	virtual void StartSimulation();
	virtual void StopSimulation();

	virtual double GetSimulationRate() const { return _stepSize; }
	virtual double GetMaxSimulationTime() const { return _maxSimulationTime; }
	virtual PxSolverType::Enum GetSolverType() const { return _solverType; }
	virtual PxBroadPhaseType::Enum GetBroadPhaseAlgorithm() const { return _broadPhaseType; }
	virtual PxFrictionType::Enum GetFrictionType() const { return _frictionType; }
	virtual PxSceneFlags GetSceneFlags() const { return _flags; }
	virtual double GetRealTimeIndex() const { return _realTimeIndex; }
	virtual double GetSimulationIndex() const { return _simIndex; }
	virtual double GetSimulationWaitingTime() const { return _simulationWait; }

	virtual void SetSimulationRate(double stepSize) { _stepSize = stepSize; }
	virtual void SetMaxSimulationTime(double timeInMS) { _maxSimulationTime = timeInMS; }
	virtual void SetSolverType(PxSolverType::Enum solverType) { _solverType = solverType; }
	virtual void SetBroadPhaseAlgorithm(PxBroadPhaseType::Enum broadPhase) { _broadPhaseType = broadPhase; }
	virtual void SetFrictionType(PxFrictionType::Enum frictionType) { _frictionType = frictionType; }
	virtual void SetSceneFlags(PxSceneFlags flags) { _flags = flags; }


	virtual void Simulate(bool sync = false);
	virtual bool FetchResults(bool block = false);

	virtual void RegisterSceneObject(PhysXSceneObject *obj) { _sceneObjects.insert(obj); }
	virtual void UnregisterSceneObject(PhysXSceneObject *obj) { _sceneObjects.erase(obj); }

	unsigned __threadEnter();

protected:
	PxScene *_scene;
	PxDefaultCpuDispatcher *_cpuDispatcher;

	bool _isSimulating;
	bool _isRunning;

	std::thread _simulationThread;
	HANDLE _threadHandle;
	HANDLE _simulateEvent;
	HANDLE _simulateDoneEvent;
	bool _terminate;

	bool __isSimulating() const { return _isSimulating; }

	bool _started;
	double _accum;

	double _stepSize;
	double _maxSimulationTime;
	double _realTimeIndex;
	double _simIndex;
	double _simulationWait;
	PxSolverType::Enum _solverType;
	PxBroadPhaseType::Enum _broadPhaseType;
	PxFrictionType::Enum _frictionType;
	PxSceneFlags _flags;

	Set<PxActor*> _updatedActors;
	Set<PhysXSceneObject*> _sceneObjects;

	void _simulate(double &realTimeIndex, double &simIndex);
	virtual void OnRelease() { DestroyScene(); }
};



}
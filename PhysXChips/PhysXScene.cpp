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
#include "PhysXScene.h"
#include "PhysX.h"
#include <mutex>

using namespace m3d;


CHIPDESCV1_DEF(PhysXScene, L"PhysX Scene", PHYSXSCENE_GUID, CHIP_GUID);


unsigned __stdcall _wakeUpdateThreadEnter(void *param) { return ((PhysXScene*)param)->__threadEnter(); }


PhysXScene::PhysXScene()
{
	_scene = nullptr;
	_cpuDispatcher = nullptr;
	_isSimulating = false;
	_isRunning = false;

	_terminate = false;

	_started = false;
	_accum = 0.0;
	_stepSize = 120.0;
	_maxSimulationTime = 100.0;
	_realTimeIndex = 1.0;
	_simIndex = 1.0;
	_simulationWait = 0.0;
	_solverType = PxSolverType::ePGS;
	_broadPhaseType = PxBroadPhaseType::eABP;
	_frictionType = PxFrictionType::ePATCH;
	_flags = PxSceneFlag::eENABLE_PCM;

	_simulateEvent = CreateEvent(0, FALSE, FALSE, 0);
	_simulateDoneEvent = CreateEvent(0, FALSE, FALSE, 0);

//	unsigned int threadID;
//	_threadHandle = (HANDLE)_beginthreadex(0, 0, _wakeUpdateThreadEnter, this, 0, &threadID);

	_simulationThread = std::move(std::thread(&PhysXScene::__threadEnter, this));


}

PhysXScene::~PhysXScene()
{
	if (_cpuDispatcher)
		_cpuDispatcher->release();
	_cpuDispatcher = nullptr;

	_terminate = true;
	_simulationThread.join();
	DWORD dw = STILL_ACTIVE;
//	if (_threadHandle)
		//while (dw == STILL_ACTIVE)
			//GetExitCodeThread(_threadHandle, &dw);

	if (_simulateEvent) CloseHandle(_simulateEvent);
	if (_simulateDoneEvent) CloseHandle(_simulateDoneEvent);
}

bool PhysXScene::CopyChip(Chip *chip)
{
	PhysXScene *c = dynamic_cast<PhysXScene*>(chip);
	B_RETURN(Chip::CopyChip(c));
	_stepSize = c->_stepSize;
	_maxSimulationTime = c->_maxSimulationTime;
	_solverType = c->_solverType;
	_broadPhaseType = c->_broadPhaseType;
	_frictionType = c->_frictionType;
	_flags = c->_flags;
	return true;
}

bool PhysXScene::LoadChip(DocumentLoader &loader)
{
	B_RETURN(Chip::LoadChip(loader));
	LOADDEF("stepSize", _stepSize, 120.0);
	LOADDEF("maxSimulationTime", _maxSimulationTime, 100.0);
	LOADDEF("solverType", _solverType, PxSolverType::ePGS);
	LOADDEF("broadPhaseType", _broadPhaseType, PxBroadPhaseType::eABP);
	LOADDEF("frictionType", _frictionType, PxFrictionType::ePATCH);
	LOADDEF("flags", (unsigned&)_flags, PxSceneFlag::eENABLE_PCM);
	return true;
}

bool PhysXScene::SaveChip(DocumentSaver &saver) const
{
	B_RETURN(Chip::SaveChip(saver));
	SAVEDEF("stepSize", _stepSize, 120.0);
	SAVEDEF("maxSimulationTime", _maxSimulationTime, 100.0);
	SAVEDEF("solverType", _solverType, PxSolverType::ePGS);
	SAVEDEF("broadPhaseType", _broadPhaseType, PxBroadPhaseType::eABP);
	SAVEDEF("frictionType", _frictionType, PxFrictionType::ePATCH);
	SAVEDEF("flags", (unsigned)_flags, PxSceneFlag::eENABLE_PCM);
	return true;
}

void PhysXScene::CallChip()
{
}

PxScene *PhysXScene::GetScene()
{
	if (_scene)
		return _scene;

	PhysXSDK *sdk = (PhysXSDK*)engine->GetChipManager()->GetGlobalChip(PHYSXSDK_GUID);

	if (!_cpuDispatcher) {
		_cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		if(!_cpuDispatcher) {}
	}

	PxSceneDesc sceneDesc(sdk->GetPhysics()->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher = _cpuDispatcher;
	sceneDesc.solverType = _solverType;
	sceneDesc.broadPhaseType = _broadPhaseType;
	sceneDesc.flags = _flags;
	sceneDesc.flags.set(PxSceneFlag::eENABLE_ACTIVE_ACTORS);

	if (_broadPhaseType == PxBroadPhaseType::eGPU || _flags.isSet(PxSceneFlag::eENABLE_GPU_DYNAMICS)) {
		PxCudaContextManager *cuda = sdk->GetCudaContextManager();

		if (cuda) {
			sceneDesc.cudaContextManager = cuda;
			if (_flags.isSet(PxSceneFlag::eENABLE_GPU_DYNAMICS)) {
				sceneDesc.flags.set(PxSceneFlag::eENABLE_GPU_DYNAMICS);
				sceneDesc.flags.set(PxSceneFlag::eENABLE_PCM);
			}
		}
		else {
			msg(WARN, L"Failed to enable GPU-accelerated PhysX.");
			sceneDesc.flags.clear(PxSceneFlag::eENABLE_GPU_DYNAMICS);
			if (_broadPhaseType == PxBroadPhaseType::eGPU)
				sceneDesc.broadPhaseType = PxBroadPhaseType::eABP; // Fallback
		}
	}

	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = &PxDefaultSimulationFilterShader;

	_scene = sdk->GetPhysics()->createScene(sceneDesc);
	_scene->userData = this;
	
	_scene->setVisualizationParameter ( PxVisualizationParameter::eSCALE , 1);
	_scene->setVisualizationParameter ( PxVisualizationParameter::eCOLLISION_SHAPES , 1);
	_scene->setVisualizationParameter ( PxVisualizationParameter::eWORLD_AXES , 1);
	_scene->setVisualizationParameter ( PxVisualizationParameter::eACTOR_AXES  , 1);
	
	_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
	_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
	
	return _scene;
}

void PhysXScene::DestroyScene()
{
	if (_isSimulating)
		FetchResults(true);
	assert(_isSimulating == false);

	if (_scene)
		_scene->release();
	_scene = nullptr;
	_isRunning = false;

	for (Set<PhysXSceneObject*>::cNode n = _sceneObjects.front(); n.valid(); n++)
		n->OnSceneDestroyed();
	_sceneObjects.clear();

/*
	// Iterate all actors and remove them!
	List<PxActor*> actors(_scene->getNbActors(PxActorTypeSelectionFlag::eRIGID_STATIC | PxActorTypeSelectionFlag::eRIGID_DYNAMIC), (PxActor*)nullptr);
	_scene->getActors(PxActorTypeSelectionFlag::eRIGID_STATIC | PxActorTypeSelectionFlag::eRIGID_DYNAMIC, &actors.front(), (physx::PxU32)actors.size());
	for (size_t i = 0; i < actors.size(); i++) {
		PhysXActorData *data = (PhysXActorData*)actors[i]->userData;
		assert(data);
		data->owner->OnSceneDestroyed();
	}
*/	
}

void PhysXScene::StartSimulation()
{
	if (!_scene)
		return;
	_isRunning = true;
}

void PhysXScene::StopSimulation()
{
	_isRunning = false;
}

std::mutex _mutex;
std::condition_variable _simulateCondition;
std::condition_variable _simulateDoneCondition;
bool _simDone =false;

void PhysXScene::Simulate(bool sync)
{
	if (_isSimulating || !_isRunning || !_scene)
		return;
	
	double dt = (double)engine->GetDt() / 1000000.0;

	_accum += dt;
	//_isSimulating = true;

	std::unique_lock<std::mutex> lock(_mutex);
	_isSimulating = true;
	_simulateCondition.notify_one();

	//	_simulate();
	//SetEvent(_simulateEvent);
}

bool PhysXScene::FetchResults(bool block)
{
	if (!_isSimulating)
		return false;

	auto start = std::chrono::high_resolution_clock::now();

	std::unique_lock<std::mutex> lock(_mutex);

	//WaitForSingleObject(_simulateDoneEvent, INFINITE);
	while (!_simDone)
		_simulateDoneCondition.wait(lock);

	auto stop = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = stop - start;
	_simulationWait = floor(diff.count() * 10000.0) / 10.0;



	// Clear forces
	PxActor *a = nullptr;
	List<PxActor*> actors(_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC), a);
	_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, &actors.front(), (physx::PxU32)actors.size());
	for (size_t i = 0; i < actors.size(); i++) {
		PhysXActorData *data = (PhysXActorData*)actors[i]->userData;
		assert(data);
		data->forces.clear();
	}

	// Update poses
	for (Set<PxActor*>::cNode n = _updatedActors.front(); n.valid(); n++) {
		PhysXActorData *d = (PhysXActorData*)n->userData;
		assert(d);
		d->globalPose = n->is<PxRigidActor>()->getGlobalPose();
	}
	_updatedActors.clear();

	_isSimulating = false;
	_simDone = false;

	return true;
}



unsigned PhysXScene::__threadEnter()
{
	while (true) {
		{
		std::unique_lock<std::mutex> lock(_mutex);

		_simulateCondition.wait_for(lock, std::chrono::milliseconds(100));
			
		if (_terminate)
			return 0;

		if (!_isSimulating)
			continue;

		}
		//while (WaitForSingleObject(_simulateEvent, 100) == WAIT_TIMEOUT) 
			//if (_terminate) 
				//return 0;

		double realTimeIndex, simIndex;
		_simulate(realTimeIndex, simIndex);

		std::unique_lock<std::mutex> lock(_mutex);
		_simDone = true;
		_realTimeIndex = realTimeIndex;
		_simIndex = simIndex;
		_simulateDoneCondition.notify_one();

		//SetEvent(_simulateDoneEvent);
		
	}
	return 0;
}

void PhysXScene::_simulate(double &realTimeIndex, double &simIndex)
{
	PxActor *a = nullptr;
	List<PxActor*> actors(_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC), a);
	_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, &actors.front(), (physx::PxU32)actors.size());

	List<PxRigidDynamic*> kinematics;

	double stepSize = 1.0 / _stepSize;

	realTimeIndex = 1.0;

	unsigned steps = unsigned(_accum / stepSize);

	auto start = std::chrono::high_resolution_clock::now();

	for (unsigned i = 0; i < steps; i++) {
		_accum -= stepSize;

		float f = float(i + 1) / steps;

		// Apply forces & move kinematics
		for (size_t i = 0; i < actors.size(); i++) {
			PhysXActorData *a = (PhysXActorData*)actors[i]->userData;
			if (a && a->actor->is<PxRigidBody>()) {
				for (size_t j = 0; j < a->forces.size(); j++) {
					const PhysXForce &f = a->forces[j];
					switch (f.frame)
					{
					case PhysXForce::GLOBAL_AT_GLOBAL_POS:
						PxRigidBodyExt::addForceAtPos(*a->actor->is<PxRigidBody>(), f.force, f.pos, f.mode);
						break;
					case PhysXForce::GLOBAL_AT_LOCAL_POS:
						PxRigidBodyExt::addForceAtLocalPos(*a->actor->is<PxRigidBody>(), f.force, f.pos, f.mode);
						break;
					case PhysXForce::LOCAL_AT_GLOBAL_POS:
						PxRigidBodyExt::addLocalForceAtPos(*a->actor->is<PxRigidBody>(), f.force, f.pos, f.mode);
						break;
					case PhysXForce::LOCAL_AT_LOCAL_POS:

						PxRigidBodyExt::addLocalForceAtLocalPos(*a->actor->is<PxRigidBody>(), f.force, f.pos, f.mode);
						break;
					case PhysXForce::GLOBAL_TORQUE:
						a->actor->is<PxRigidBody>()->addTorque(f.force, f.mode);
						break;
					case PhysXForce::LOCAL_TORQUE:
						a->actor->is<PxRigidBody>()->addTorque(a->actor->is<PxRigidBody>()->getGlobalPose().transform(f.force), f.mode);
						break;
					}
				}

				if (a->moveKinematic) {
					PxTransform t;
					t.p = a->globalPose.p + (a->kinematicTarget.p - a->globalPose.p) * f;
					XMStoreFloat4((XMFLOAT4*)&t.q, XMQuaternionSlerp(XMLoadFloat4((XMFLOAT4*)&a->globalPose.q), XMLoadFloat4((XMFLOAT4*)&a->kinematicTarget.q), f));
					a->actor->is<PxRigidDynamic>()->setKinematicTarget(t);
					if (i == steps - 1)
						a->moveKinematic = false;
				}
			}
		}

		_scene->simulate((physx::PxReal)stepSize);

		// Callbacks before swapping: onTrigger, onContact, onConstraintBreak 
		// Callbacks after swapping: onSleep, onWake 
		bool b = _scene->fetchResults(true);

		// Mark updated objects.
		PxU32 nbActiveActors = 0;
		PxActor **activeActors = _scene->getActiveActors(nbActiveActors);
		for (PxU32 i = 0; i < nbActiveActors; i++)
			_updatedActors.insert(activeActors[i]);

		auto stop = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> diff = stop - start;

		// Is total time used so far + estimated time for next step larger than max allowed simulation time?
		if (i < (steps - 1) && (diff.count() / (i + 1) * (i + 2)) > (_maxSimulationTime * 0.001)) {
			double workDone = stepSize * (i + 1);
			realTimeIndex = workDone / (workDone + _accum);
			_accum = 0.0;
			break;
		}
	}

	auto stop = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff = stop - start;

	simIndex = diff.count() / (_maxSimulationTime * 0.001);
}
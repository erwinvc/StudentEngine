#include "stdafx.h"

StateLoadJob::StateLoadJob(BaseState* state) : AssetLoadJob(state->GetName()), m_state(state) {}


bool StateLoadJob::LoadAsset(bool addToProcessQueue) {
	return true;
};
void StateLoadJob::ProcessAsset(map<String, AssetBase*>& assets) {
	Timer timer;
	m_state->Initialize();
	LOG("[~mGameState~x] initialized ~1%s~x in %.2fms", m_state->GetName().c_str(), timer.Get());
}
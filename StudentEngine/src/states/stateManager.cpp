#include "stdafx.h"

void StateManager::Initialize() {
	if (m_initialized) {
		return;
	}
	SetState(EDIT);
	m_initialized = true;
}

void StateManager::SetState(State newState) {
	switch (newState) {
	case EDIT:
		LOG("[~GStates~x] Switched to ~1EDIT~x state");
		break;
	case PLAY:
		LOG("[~GStates~x] Switched to ~1PLAY~x state");
		break;
	default:
		break;
	}
	m_state = newState;
}

State StateManager::GetState() {
	return m_state;
}
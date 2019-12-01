#include "stdafx.h"

void StateManager::Initialize() {
	if (m_initialized) {
		return;
	}
	SetState<EditState>();
	m_initialized = true;
}

BaseState* StateManager::GetState() {
	return m_state;
}
#include "stdafx.h"

BaseState* States::LOADING = nullptr;
BaseState* States::EDIT = nullptr;
BaseState* States::PLAY = nullptr;

void StateManager::Initialize() {
	if (m_initialized) {
		return;
	}

	States::LOADING = RegisterState<LoadingState>();
	States::EDIT = RegisterState<EditState>();
	States::PLAY = RegisterState<PlayState>();
	States::LOADING->Initialize();
	SetState(States::LOADING);
	m_initialized = true;
}

BaseState* StateManager::GetState() {
	return m_state;
}
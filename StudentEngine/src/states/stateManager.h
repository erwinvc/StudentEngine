#pragma once

enum State : int {
	EDIT = 0,
	PLAY = 1
};

class StateManager : public Singleton<StateManager>
{
private:
	bool m_initialized;
	State m_state;
public:
	void Initialize();
	void SetState(State newState);
	State GetState();
};

static StateManager* GetStateManager() {
	return StateManager::GetInstance();
}
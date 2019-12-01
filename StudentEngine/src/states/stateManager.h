#pragma once
class StateManager : public Singleton<StateManager>
{
private:
	bool m_initialized;
	BaseState* m_state;

	template<typename T>
	T* CreateState() {
		return new T();
	}
public:
	void Initialize();
	BaseState* GetState();

	template<typename T>
	void SetState() {
		BaseState* newState = CreateState<T>(); //#TODO mem leak?
		if (newState != nullptr) {
			if (m_state != nullptr) {
				m_state->ExitState();
			}
			delete m_state;
			m_state = newState;
			m_state->EnterState();
		}
	}
};

static StateManager* GetStateManager() {
	return StateManager::GetInstance();
}
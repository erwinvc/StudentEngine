#pragma once

namespace States {
	extern BaseState* LOADING;
	extern BaseState* EDIT;
	extern BaseState* PLAY;
}

class StateManager : public Singleton<StateManager> {
protected:
	StateManager() {}
	~StateManager() {
		for (auto& state : m_states) delete state;
	}
	friend Singleton;

private:
	bool m_initialized;
	BaseState* m_state;
	vector<BaseState*> m_states;

	template<typename T>
	BaseState* RegisterState() {
		for (BaseState* state : m_states) {
			if (typeid(state) == typeid(T)) {
				LOG_ERROR("[~mGameState~x] already registered ~1%s~x", state->GetName().c_str());
				return (T*)state;
			}
		}
		T* instance = new T();

		m_states.push_back(instance);
		LOG("[~mGameState~x] registered ~1%s ~xstate", instance->GetName().c_str());
		return (T*)instance;
	}

public:
	void Initialize();
	BaseState* GetState();

	void SetState(BaseState* state) {
		if (m_state != nullptr) {
			m_state->ExitState();
		}
		m_state = state;
		m_state->EnterState();
	}

	vector<BaseState*>& GetStates() { return m_states; }

	void Update(const TimeStep& time) {
		m_state->Update(time);
	}

	void Draw(RenderingPipeline* pipeline) {
		m_state->Draw(pipeline);
	}

	void PostDraw(RenderingPipeline* pipeline) {
		m_state->PostDraw(pipeline);
	}
	void OnImGui() {
		m_state->OnImGui();
	}
};

static StateManager* GetStateManager() {
	return StateManager::GetInstance();
}
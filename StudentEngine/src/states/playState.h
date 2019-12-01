#pragma once

class PlayState : public BaseState {
private:
	vector<GameObject> editorGameObjects;
public:
	PlayState();
	void EnterState() override;
	void ExitState() override;
};
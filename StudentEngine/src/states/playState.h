#pragma once

class PlayState : public BaseState {
private:
	vector<GameObject> editorGameObjects;
public:
	PlayState();
	void Initialize() override;
	void Update(const TimeStep& time) override;
	void Draw(RenderingPipeline* pipeline) override;
	void EnterState() override;
	void ExitState() override;
	void OnImGui() override;
};
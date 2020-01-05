#pragma once

class PlayState : public BaseState {
private:
	Hierarchy* m_hierarchy;
	vector<GameObject> editorGameObjects;
	Camera* m_editorCamera;
	Camera* m_playCamera;
	bool m_restarting;
public:
	PlayState() : BaseState("Play") {}
	~PlayState();
	void Initialize() override;
	void Update(const TimeStep& time) override;
	void Draw(RenderingPipeline* pipeline) override;
	void PostDraw(RenderingPipeline* pipeline) override;
	void PostImGuiDraw(RenderingPipeline* pipeline) override;
	void EnterState() override;
	void ExitState() override;
	void OnImGui() override;
	void Restart();
};
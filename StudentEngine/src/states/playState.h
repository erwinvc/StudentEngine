#pragma once

class PlayState : public BaseState {
private:
	Hierarchy* m_hierarchy;
	vector<GameObject> editorGameObjects;
	Camera* m_editorCamera;
	Camera* m_playCamera;
	bool m_restarting = false;
	bool m_gameover;
	bool m_finished;
	bool m_paused;
	int m_playerScore = 0;
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
	void OnHUD();
	void AdjustScore(int value);
	void Restart();
	void GameOver();
	void Victory();
};
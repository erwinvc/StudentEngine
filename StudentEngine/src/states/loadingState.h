#pragma once

class LoadingState : public BaseState
{
private:
	StreamedTexture* m_loadingTexture[4];
	float m_transparency = 1.0f;
	bool m_fade = false;
public:
	LoadingState() : BaseState("Loading") {}
	void Initialize() override;
	void Update(const TimeStep& time) override;
	void Draw(RenderingPipeline* pipeline) override;
	void PostDraw(RenderingPipeline* pipeline) override;
	void EnterState() override;
	void ExitState() override;
	void OnImGui() override;
};
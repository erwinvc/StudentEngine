#pragma once

class LoadingState : public BaseState
{
private:
	StreamedTexture* m_loadingTexture[4];
public:
	void Initialize() override;
	void Update(const TimeStep& time) override;
	void Draw(RenderingPipeline* pipeline) override;
	void EnterState() override;
	void ExitState() override;
	void OnImGui() override;
};
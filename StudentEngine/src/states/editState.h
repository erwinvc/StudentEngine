#pragma once

class EditState : public BaseState {
public:
	EditState() : BaseState("Edit") {}

	void Initialize() override;
	void Update(const TimeStep& time) override;
	void Draw(RenderingPipeline* pipeline) override;
	void PostDraw(RenderingPipeline* pipeline) override;
	void EnterState() override;
	void ExitState() override;
	void OnImGui() override;
};
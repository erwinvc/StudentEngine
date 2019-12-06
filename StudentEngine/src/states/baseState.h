#pragma once

class BaseState {
private:
	String m_name;

public:
	BaseState(const String& name) : m_name(name) {}

	const String& GetName() { return m_name; }
	virtual void Initialize() = 0;
	virtual void EnterState() = 0;
	virtual void ExitState() = 0;
	virtual void Update(const TimeStep& time) = 0;
	virtual void Draw(RenderingPipeline* pipeline) = 0;
	virtual void PostDraw(RenderingPipeline* pipeline) = 0;
	virtual void PostImGuiDraw(RenderingPipeline* pipeline) = 0;
	virtual void OnImGui() = 0;
};
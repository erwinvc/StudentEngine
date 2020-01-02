#pragma once

class BaseState {
protected:
	String m_name;
	Scene* m_scene;
public:
	BaseState(const String& name) : m_name(name), m_scene(new Scene()) {}
	virtual ~BaseState() { delete m_scene; }
	const String& GetName() { return m_name; }
	virtual Scene* GetScene() { return m_scene; }
	virtual void Initialize() = 0;
	virtual void EnterState() = 0;
	virtual void ExitState() = 0;
	virtual void Update(const TimeStep& time) = 0;
	virtual void Draw(RenderingPipeline* pipeline) = 0;
	virtual void PostDraw(RenderingPipeline* pipeline) = 0;
	virtual void PostImGuiDraw(RenderingPipeline* pipeline) = 0;
	virtual void OnImGui() = 0;
};
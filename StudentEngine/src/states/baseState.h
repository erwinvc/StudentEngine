#pragma once

class BaseState {
private:
	String name;
	
public:
	virtual void Initialize() = 0;
	virtual void EnterState() = 0;
	virtual void ExitState() = 0;
	virtual void Update(const TimeStep& time) = 0;
	virtual void Draw(RenderingPipeline* pipeline) = 0;
	virtual void OnImGui() = 0;
	const String& GetState();
};
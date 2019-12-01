#pragma once

class BaseState {
private:
	String name;
public:
	virtual void EnterState() = 0;
	virtual void ExitState() = 0;
	String GetState();
};
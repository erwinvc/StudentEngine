#pragma once

class BaseState;

class StateLoadJob : public AssetLoadJob {
private:
	BaseState* m_state;
public:
	StateLoadJob(BaseState* state);

	bool loadAsset(bool addToProcessQueue = true) override;
	void processAsset(map<String, AssetBase*>& assets) override;
};
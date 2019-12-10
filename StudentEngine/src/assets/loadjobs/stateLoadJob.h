#pragma once

class BaseState;

class StateLoadJob : public AssetLoadJob {
private:
	BaseState* m_state;
public:
	StateLoadJob(BaseState* state);

	bool LoadAsset(bool addToProcessQueue = true) override;
	void ProcessAsset(map<String, AssetBase*>& assets) override;
};
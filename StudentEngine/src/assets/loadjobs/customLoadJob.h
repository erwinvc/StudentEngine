#pragma once

class CustomLoadJob : public AssetLoadJob {
private:
	function<void(void)> m_aSync;
	function<void(void)> m_sync;
public:
	CustomLoadJob(const String& name, function<void(void)> aSync, function<void(void)> sync = Utils::nullfunc) : AssetLoadJob(name), m_aSync(aSync), m_sync(sync) {}
	bool LoadAsset(bool addToProcessQueue = true) override {
		m_aSync();
		return true;
	}

	void ProcessAsset(map<String, AssetBase*>& assets) override {
		m_sync();
	}
};
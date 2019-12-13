#pragma once
class AudioLoadJob : public AssetLoadJob
{
private:
	Path m_filePath;
public:
	AudioLoadJob(const String& id, const Path& filePath);
	bool LoadAsset(bool addToProcessQueue = true) override;
	void ProcessAsset(map<String, AssetBase*>& assets) override;
};
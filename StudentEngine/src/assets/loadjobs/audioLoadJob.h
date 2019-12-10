#pragma once
#define MINIMP3_IMPLEMENTATION

class AudioLoadJob : public AssetLoadJob
{
private:
	Path m_filePath;
	AudioType m_type;

	int LoadMp3();
public:
	AudioLoadJob(const String& id, const Path& filePath, const AudioType& type);
	~AudioLoadJob();
	bool LoadAsset(bool addToProcessQueue = false) override;
	void ProcessAsset(map<String, AssetBase*>& assets) override;
};
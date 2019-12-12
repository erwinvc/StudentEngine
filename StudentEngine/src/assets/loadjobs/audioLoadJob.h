#pragma once
#define MINIMP3_IMPLEMENTATION

class AudioLoadJob : public AssetLoadJob
{
private:
	Path m_filePath;
	AudioType m_type;
	Audio* newAsset;
	//int LoadMp3();
	void LoadWav();
public:
	AudioLoadJob(const String& id, const Path& filePath, const AudioType& type);
	~AudioLoadJob();
	bool LoadAsset(bool addToProcessQueue = true) override;
	void ProcessAsset(map<String, AssetBase*>& assets) override;
};
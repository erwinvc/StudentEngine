#pragma once
class TextureLoadJob : public AssetLoadJob
{
private:
	TextureParameters m_params;
	Path m_filePath;
	byte* m_data;
	int m_width;
	int m_height;
	int m_channelCount;
public:
	TextureLoadJob(const String& id, uint32 width, uint32 height, byte* data, const TextureParameters& params = TextureParameters());
	TextureLoadJob(const String& id, const Path& filePath, const TextureParameters& params = TextureParameters());
	~TextureLoadJob();
	bool LoadAsset(bool addToProcessQueue = true) override;
	void ProcessAsset(map<String, AssetBase*>& assets) override;
};
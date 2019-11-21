#pragma once
class TextureLoadJob : public AssetLoadJob
{
private:
	TextureParameters m_params;
	String m_filePath;
	byte* m_data;
	int m_width;
	int m_height;
	int m_channelCount;
public:
	TextureLoadJob(const String& id, const String& filePath, const TextureParameters& params = TextureParameters());
	~TextureLoadJob();
	void loadAsset() override;
	void processAsset(map<String, AssetBase*>& assets) override;
};
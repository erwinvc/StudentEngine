#pragma once
enum class AudioType {
	WAV,
	MP3
};

class Audio : public AssetBase
{
public:
	Audio(Path& filePath, void* data, uint64_t size, uint32_t channels, uint32_t sampleRate);
	~Audio();
	void* GetData();
	uint64_t GetSize();
	uint32_t GetChannels();
	uint32_t GetSampleRate();
private:
	void* m_data;
	uint64_t m_size;
	uint32_t m_channels;
	uint32_t m_sampleRate;
	Path m_filePath;
};
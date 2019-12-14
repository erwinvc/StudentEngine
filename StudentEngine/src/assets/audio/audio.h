#pragma once

class Audio : public AssetBase {
private:
	SoLoud::Wav m_wav;
public:
	Audio(const Path& filePath);
	~Audio();
	SoLoud::Wav& GetWav();
};
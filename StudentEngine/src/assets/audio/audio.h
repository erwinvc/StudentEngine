#pragma once

class Audio : public AssetBase {
private:
	SoLoud::Wav m_wav;
public:
	Audio(const String& name, const Path& filePath);
	~Audio();
	SoLoud::Wav& GetWav();
	Texture* GetTexture() override;
};
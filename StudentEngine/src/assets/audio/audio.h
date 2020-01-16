#pragma once

class Audio : public AssetBase {
private:
	SoLoud::Wav m_wav;
	map<string, int> m_handles;
public:
	Audio(const String& name, const Path& filePath);
	~Audio();
	SoLoud::Wav& GetWav();
	Texture* GetTexture() override;
	void RegisterHandle(GameObject* owner, int handle);
	int GetHandle(GameObject* owner);
	void SetLooping(bool loop);
};
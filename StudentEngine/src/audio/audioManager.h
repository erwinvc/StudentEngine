#pragma once

class AudioManager : public Singleton<AudioManager> {
private:
	SoLoud::Soloud audioCore;
public:
	void Initialize();
	void Play(const Path& filePath);
};

static AudioManager* GetAudioManager() {
	return AudioManager::GetInstance();
}
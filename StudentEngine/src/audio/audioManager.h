#pragma once

class AudioManager : public Singleton<AudioManager> {
private:
	SoLoud::Soloud audioCore;
public:
	void Initialize();
	void Cleanup();
	void Play(Audio* audio);
};

static AudioManager* GetAudioManager() {
	return AudioManager::GetInstance();
}
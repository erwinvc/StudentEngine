#pragma once

class AudioManager : public Singleton<AudioManager> {
private:
	SoLoud::Soloud audioCore;
public:
	void Initialize();
	void Cleanup();
	void Play(GameObject* owner, Audio* audio);
	void Stop(GameObject* owner, Audio* audio);
	void StopAll();
	void Pause(GameObject* owner, Audio* audio);
	void Unpause(GameObject* owner, Audio* audio);
	bool IsPlaying(GameObject* owner, Audio* audio);
};

static AudioManager* GetAudioManager() {
	return AudioManager::GetInstance();
}
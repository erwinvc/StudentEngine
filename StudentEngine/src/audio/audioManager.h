#pragma once
class AudioManager : public Singleton<AudioManager>
{
private:
	ALCdevice* m_device;
	ALCcontext* m_context;
	ALenum m_error;

	//ALuint* CreateBuffers(int bufferAmount);
public:
	AudioManager();
	~AudioManager();

	friend Singleton;

	void Initialize();
	void PlayAudio(Audio* audio);
	void LogError(const char* id, ALenum& error);
};

static AudioManager* GetAudioManager() {
	return AudioManager::GetInstance();
}


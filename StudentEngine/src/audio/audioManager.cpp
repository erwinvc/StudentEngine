#include "stdafx.h"

void AudioManager::Initialize() {
	audioCore.init();
}

void AudioManager::Cleanup() {
	audioCore.deinit();
}

void AudioManager::Play(GameObject* owner, Audio* audio) {
	int handle = audioCore.play(audio->GetWav());
	audio->RegisterHandle(owner, handle);
}

void AudioManager::Stop(GameObject* owner, Audio* audio) {
	audioCore.stop(audio->GetHandle(owner));
}

void AudioManager::StopAll() {
	audioCore.stopAll();
}

void AudioManager::Pause(GameObject* owner, Audio* audio) {
	audioCore.setPause(audio->GetHandle(owner), true);
}

void AudioManager::Unpause(GameObject* owner, Audio* audio) {
	audioCore.setPause(audio->GetHandle(owner), false);
}

bool AudioManager::IsPlaying(GameObject* owner, Audio* audio) {
	return audioCore.isValidVoiceHandle(audio->GetHandle(owner));
}
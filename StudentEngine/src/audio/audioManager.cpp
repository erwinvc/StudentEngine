#include "stdafx.h"

void AudioManager::Initialize() {
	audioCore.init();
}

void AudioManager::Cleanup() {
	audioCore.deinit();
}

void AudioManager::Play(Audio* audio) {
	audioCore.play(audio->GetWav());
	while (audioCore.getActiveVoiceCount() > 0) {
		SoLoud::Thread::sleep(100);
	}
}
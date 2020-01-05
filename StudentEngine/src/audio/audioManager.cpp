#include "stdafx.h"

void AudioManager::Initialize() {
	audioCore.init();
}

void AudioManager::Cleanup() {
	audioCore.deinit();
}

void AudioManager::Play(Audio* audio) {
	if (!audioCore.getActiveVoiceCount() > 0) {
		audioCore.play(audio->GetWav());
	}
}
#include "stdafx.h"

void AudioManager::Initialize() {
	audioCore.init();
}

void AudioManager::Cleanup() {
	audioCore.deinit();
}

void AudioManager::Play(Audio* audio) {
	audioCore.play(audio->GetWav());
	SoLoud::Thread::sleep(audio->GetWav().getLength());
}
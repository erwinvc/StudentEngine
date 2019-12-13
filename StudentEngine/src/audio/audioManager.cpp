#include "stdafx.h"

void AudioManager::Initialize() {
	audioCore.init();
}

void AudioManager::Play(const Path& filePath) {
	SoLoud::Wav waveFile;
	waveFile.load(filePath.GetFullPath().c_str());
	audioCore.play(waveFile);
	while (audioCore.getActiveVoiceCount() > 0) {
		SoLoud::Thread::sleep(100);
	}
}
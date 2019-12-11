#include "stdafx.h"
#include "vendor/minimp3_ex.h"

AudioLoadJob::AudioLoadJob(const String& id, const Path& filePath, const AudioType& type) :
	AssetLoadJob(id), m_filePath(filePath), m_type(type) {};

AudioLoadJob::~AudioLoadJob() {

}

bool AudioLoadJob::LoadAsset(bool addToProcessQueue) {
	switch (m_type) {
	case AudioType::MP3:
		if (LoadMp3()) {
			LOG_ERROR("[Audio] Loading MP3 failed!");
		}
		else {
			LOG("[~BAudio~x] Loaded ~1%s~x", m_filePath.GetFullPath().c_str());
		}
		break;
	default:
		break;
	}
	return addToProcessQueue;
}

void AudioLoadJob::ProcessAsset(map<String, AssetBase*>& assets) {
}

int AudioLoadJob::LoadMp3() {
	mp3dec_t mp3d;
	mp3dec_file_info_t info;

	if (mp3dec_load(&mp3d, m_filePath.GetFullPath().c_str(), &info, NULL, NULL)) {
		return -1;
	}
	else {
		free(info.buffer);
		return 0;
	}
}
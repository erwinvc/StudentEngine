#include "stdafx.h"
#define DR_WAV_IMPLEMENTATION
#include "vendor/dr_wav.h"

AudioLoadJob::AudioLoadJob(const String& id, const Path& filePath, const AudioType& type) :
	AssetLoadJob(id), m_filePath(filePath), m_type(type) {};

AudioLoadJob::~AudioLoadJob() {
	delete newAsset;
}

bool AudioLoadJob::LoadAsset(bool addToProcessQueue) {
	switch (m_type) {
	case AudioType::MP3:
		break;
	case AudioType::WAV:
		LoadWav();
		break;
	default:
		break;
	}
	return addToProcessQueue;
}

void AudioLoadJob::ProcessAsset(map<String, AssetBase*>& assets) {
	assets[m_id] = new Audio(m_filePath, newAsset->GetData(), newAsset->GetSize(), newAsset->GetChannels(), newAsset->GetSampleRate());
	LOG("[~BAudio~x] Processed ~1%s~x", m_filePath.GetFullPath().c_str());
}

void AudioLoadJob::LoadWav() {
	drwav* wavLoader = new drwav();
	if (!drwav_init_file(wavLoader, m_filePath.GetFullPath().c_str(), NULL)) {
		LOG_ERROR("[Audio] OpenAL error (WAV Loader): Failed initializing file");
	}

	int16_t* sampleData = (int16_t*) malloc(wavLoader->totalPCMFrameCount * wavLoader->channels * sizeof(int16_t));
	drwav_read_pcm_frames_s16(wavLoader, wavLoader->totalPCMFrameCount, sampleData);

	newAsset = new Audio(m_filePath, sampleData, wavLoader->totalPCMFrameCount, wavLoader->channels, wavLoader->sampleRate);

	free(sampleData);
	drwav_uninit(wavLoader);
	delete wavLoader;
}

//int AudioLoadJob::LoadMp3() {
//	mp3dec_t mp3d;
//	mp3dec_file_info_t info;
//
//	if (mp3dec_load(&mp3d, m_filePath.GetFullPath().c_str(), &info, NULL, NULL)) {
//		return -1;
//	}
//	else {
//		newAsset = new Audio(m_filePath, info.buffer, info.samples, info.channels, info.hz);
//		LOG("%d", info.hz);
//		LOG("%d", info.layer);
//		LOG("%d", info.avg_bitrate_kbps);
//		free(info.buffer);
//		return 0;
//	}
//}
#include "stdafx.h"

AudioLoadJob::AudioLoadJob(const String& id, const Path& filePath) :
	AssetLoadJob(id), m_filePath(filePath) {
}

bool AudioLoadJob::LoadAsset(bool addToProcessQueue) {
	return addToProcessQueue;
}

void AudioLoadJob::ProcessAsset(map<String, AssetBase*>& assets) {
	assets[m_id] = new Audio(m_filePath);
}
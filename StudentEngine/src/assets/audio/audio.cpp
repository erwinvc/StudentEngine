#include "stdafx.h"

Audio::Audio(Path& filePath, void* data, uint64_t size, uint32_t channels, uint32_t sampleRate) :
	m_filePath(filePath), m_data(data), m_size(size), m_channels(channels), m_sampleRate(sampleRate) {
	m_data = malloc(size);
	memcpy(data, m_data, size);
}

Audio::~Audio() {
	free(m_data);
}

void* Audio::GetData() {
	return m_data;
}

size_t Audio::GetSize() {
	return m_size;
}

uint32_t Audio::GetChannels() {
	return m_channels;
}

uint32_t Audio::GetSampleRate() {
	return m_sampleRate;
}
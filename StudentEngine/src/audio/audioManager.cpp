#include "stdafx.h"

AudioManager::AudioManager() {

}

AudioManager::~AudioManager() {
	m_context = alcGetCurrentContext();
	m_device = alcGetContextsDevice(m_context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(m_context);
	alcCloseDevice(m_device);
}

void AudioManager::Initialize() {
	m_device = alcOpenDevice(NULL);
	if (m_device) {
		LOG("[~BAudio~x] Created Audio Device");
		m_context = alcCreateContext(m_device, NULL);
		alcMakeContextCurrent(m_context);
	}
}

ALuint* AudioManager::CreateBuffers(int bufferAmount) {
	ALuint* buffers;
	alGetError();
	alGenBuffers(bufferAmount, buffers);
	if ((m_error = alGetError()) != AL_NO_ERROR) {
		LogError("Buffers", m_error);
		return nullptr;
	}
	return buffers;
}

void AudioManager::LogError(const char* id, ALenum& error) {
	const char* errorMessage;
	switch (error) {
	case AL_NO_ERROR:
		errorMessage = "No error";
		break;
	case AL_INVALID_NAME:
		errorMessage = "Bad name passed to OpenAL function";
		break;
	case AL_INVALID_ENUM:
		errorMessage = "Invalid enum value pased to OpenAL function";
		break;
	case AL_INVALID_VALUE:
		errorMessage = "Invalid value pased to OpenAL function";
		break;
	case AL_INVALID_OPERATION:
		errorMessage = "Requested OpenAL operation is not valid";
		break;
	case AL_OUT_OF_MEMORY:
		errorMessage = "Requested OpenAL operation resulted in running out of memory";
		break;
	default:
		errorMessage = "Unknown error";
		break;
	}
	LOG_ERROR("[Audio] OpenAL error (%s): %s", id, errorMessage);
}
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
	if (!m_device) {
		LOG_ERROR("[~BAudio~x] Failed opening Audio Device");
	}
	LOG("[~BAudio~x] Created Audio Device (%s)", alcGetString(m_device, ALC_DEFAULT_DEVICE_SPECIFIER));
	m_context = alcCreateContext(m_device, NULL);
	if (!m_context || alcMakeContextCurrent(m_context) == ALC_FALSE) {
		alcCloseDevice(m_device);
		LOG_ERROR("[~BAudio~x] Failed creating Audio Context");
	}
}

//ALuint* AudioManager::CreateBuffers(int bufferAmount) {
//	ALuint* buffers = 0;
//	alGetError();
//	alGenBuffers(bufferAmount, &buffers);
//	if ((m_error = alGetError()) != AL_NO_ERROR) {
//		LogError("Create Buffers", m_error);
//		return nullptr;
//	}
//	return &buffers;
//}
  

void AudioManager::PlayAudio(Audio* audio) {
	LOG("Start playing audio");
	ALuint buffer;

	alGetError();
	alGenBuffers(1, &buffer);
	if ((m_error = alGetError()) != AL_NO_ERROR) {
		LogError("Create Buffers", m_error);
	}

	ALenum format = (audio->GetChannels() == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

	alGetError();
	alBufferData(buffer, format, audio->GetData(), (ALsizei) audio->GetSize(), (ALsizei) audio->GetSampleRate());
	if ((m_error = alGetError()) != AL_NO_ERROR) {
		LogError("Load Buffer Data", m_error);
	}

	ALuint source;
	alGetError();
	alGenSources(1, &source);
	if ((m_error = alGetError()) != AL_NO_ERROR)
	{
		LogError("Create Sources", m_error);
	}

	alGetError();
	alSourcei(source, AL_BUFFER, buffer);
	if ((m_error = alGetError()) != AL_NO_ERROR)
	{
		LogError("Attach buffer to source", m_error);
	}

	alGetError();
	alSourcePlay(source);
	ALenum state = AL_PLAYING;
	while (alGetError() == AL_NO_ERROR && state == AL_PLAYING) {
		Sleep(1000);
		alGetSourcei(source, AL_SOURCE_STATE, &state);
	}

	if ((m_error = alGetError()) != AL_NO_ERROR) {
		LogError("Playing Sound", m_error);
	}

	alDeleteBuffers(1, &buffer);
	alDeleteSources(1, &source);
	LOG("Finish playing audio");
}

void AudioManager::LogError(const char* id, ALenum& error) {
	int errorCode = error;
	const char* errorMessage;
	switch (error) {
	case AL_NO_ERROR:
		errorMessage = "No error";
		break;
	case AL_INVALID_NAME:
		errorMessage = "Bad name passed to OpenAL function";
		break;
	case AL_INVALID_ENUM:
		errorMessage = "Invalid enum value passed to OpenAL function";
		break;
	case AL_INVALID_VALUE:
		errorMessage = "Invalid value passed to OpenAL function";
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
	LOG_ERROR("[Audio] OpenAL errorcode %d (%s): %s", errorCode, id, errorMessage);
}
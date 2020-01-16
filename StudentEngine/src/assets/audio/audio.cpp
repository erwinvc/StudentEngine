#include "stdafx.h"

Audio::Audio(const String& name, const Path& filePath) : AssetBase(name, filePath) {
	m_wav.load(filePath.GetFullPath().c_str());
}

Audio::~Audio() {

}

SoLoud::Wav& Audio::GetWav() {
	return m_wav;
}

Texture* Audio::GetTexture() {
	return GetAssetManager()->Get<StreamedTexture>("PlayerCat")->GetTexture();
}

void Audio::RegisterHandle(GameObject* owner, int handle) {
	m_handles[owner->m_name] = handle;
}

int Audio::GetHandle(GameObject* owner) {
	return m_handles[owner->m_name];
}

void Audio::SetLooping(bool loop) {
	LOG("%f", m_wav.getLoopPoint());
	m_wav.setLooping(loop);
}
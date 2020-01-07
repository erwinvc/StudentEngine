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
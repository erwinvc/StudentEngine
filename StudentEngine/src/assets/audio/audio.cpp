#include "stdafx.h"

Audio::Audio(const Path& filePath) : AssetBase(filePath) {
	m_wav.load(filePath.GetFullPath().c_str());
}

Audio::~Audio() {

}

SoLoud::Wav& Audio::GetWav() {
	return m_wav;
}
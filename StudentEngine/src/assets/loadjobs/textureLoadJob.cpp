#include "stdafx.h"

TextureLoadJob::TextureLoadJob(const String& id, const String& filePath, const TextureParameters& params)
	: AssetLoadJob(id), m_filePath(filePath), m_params(params) {
	m_width = 0;
	m_height = 0;
	m_channelCount = 0;
	m_data = nullptr;
}

TextureLoadJob::~TextureLoadJob() {
	delete[] m_data;
}

void TextureLoadJob::loadAsset() {
	TextureUtils::LoadTexture(m_filePath, m_params.GetFlipY(), [this](const LoadedTexture& data) {
		m_width = data.m_width;
		m_height = data.m_height;
		m_channelCount = data.m_channelCount;
		int size = m_height * m_width * 4;
		m_data = new byte[size];
		memcpy(m_data, data.m_data, size);
	});
	GetAssetManager()->AddToProcessQueue(this);
}

void TextureLoadJob::processAsset(map<String, AssetBase*>& assets) {
	if (m_data != nullptr) {
		assets[m_id] = new Texture(m_width, m_height, m_data, false, m_params);
	}
}
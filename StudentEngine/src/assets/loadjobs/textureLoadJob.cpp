#include "stdafx.h"

TextureLoadJob::TextureLoadJob(const String& id, uint32 width, uint32 height, byte* data, const TextureParameters& params)
	: AssetLoadJob(id), m_params(params), m_filePath(""), m_data(data), m_width(width), m_height(height), m_channelCount(0) {
}

TextureLoadJob::TextureLoadJob(const String& id, const String& filePath, const TextureParameters& params)
	: AssetLoadJob(id), m_params(params), m_filePath(filePath), m_data(nullptr), m_width(0), m_height(0), m_channelCount(0) {
}


TextureLoadJob::~TextureLoadJob() {
	delete[] m_data;
}

void TextureLoadJob::loadAsset(bool addToProcessQueue) {
	if (!m_data) {
		TextureUtils::LoadTexture(m_filePath, m_params.GetFlipY(), [this](const LoadedTexture& data) {
			m_width = data.m_width;
			m_height = data.m_height;
			m_channelCount = data.m_channelCount;
			int size = m_height * m_width * 4;
			m_data = new byte[size];
			memcpy(m_data, data.m_data, size);
		});
	}
	if (addToProcessQueue) GetAssetManager()->AddToProcessQueue(this);
}

void TextureLoadJob::processAsset(map<String, AssetBase*>& assets) {
	if (m_data != nullptr) {
		assets[m_id] = new Texture(m_width, m_height, m_data, false, m_params);
		LOG("[~yAssets~x] asset ~1%s~x of type ~1%s~x processed", m_id.c_str(), typeid(*assets[m_id]).name());
	}
}
#include "stdafx.h"

TextureLoadJob::TextureLoadJob(const String& id, uint32 width, uint32 height, byte* data, const TextureParameters& params)
	: AssetLoadJob(id), m_params(params), m_filePath(Path()), m_data(data), m_width(width), m_height(height), m_channelCount(0) {
	GetAssetManager()->Get<StreamedTexture>(id);
}

TextureLoadJob::TextureLoadJob(const String& id, const Path& filePath, const TextureParameters& params)
	: AssetLoadJob(id), m_params(params), m_filePath(filePath), m_data(nullptr), m_width(0), m_height(0), m_channelCount(0) {
	GetAssetManager()->Get<StreamedTexture>(id);
}

TextureLoadJob::~TextureLoadJob() {
	delete[] m_data;
}

bool TextureLoadJob::LoadAsset(bool addToProcessQueue) {
	if (!m_data) {
		TextureUtils::LoadTexture(m_filePath.GetFullPath(), m_params.GetFlipY(), [this](const LoadedTexture& data) {
			m_width = data.m_width;
			m_height = data.m_height;
			m_channelCount = data.m_channelCount;
			int size = m_height * m_width * 4;
			m_data = new byte[size];
			memcpy(m_data, data.m_data, size);
		});
	}

	return addToProcessQueue;
}

void TextureLoadJob::ProcessAsset(map<String, AssetBase*>& assets) {
	if (m_data != nullptr) {
		StreamedTexture* st = GetAssetManager()->Get<StreamedTexture>(m_id);
		st->FinishStreaming(m_filePath, new Texture(m_id, m_width, m_height, m_data, m_params));
		//LOG("[~yAssets~x] asset ~1%s~x of type ~1%s~x processed", m_id.c_str(), typeid(*assets[m_id]).name());
	}
}
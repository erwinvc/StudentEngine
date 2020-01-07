#include "stdafx.h"

StreamedTexture::StreamedTexture(const String& name) : AssetBase(name), m_texture(GetAssetManager()->GetNullTexture()), m_streamed(false) {}
StreamedTexture::StreamedTexture(const String& name, Texture* texture, bool streamed) : AssetBase(name), m_texture(texture), m_streamed(streamed) {}
StreamedTexture::~StreamedTexture() {
	if (m_streamed) delete m_texture;
}
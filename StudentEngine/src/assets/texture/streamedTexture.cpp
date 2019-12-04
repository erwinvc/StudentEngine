#include "stdafx.h"

StreamedTexture::StreamedTexture() : m_texture(GetAssetManager()->GetNullTexture()), m_streamed(false) {}
StreamedTexture::StreamedTexture(Texture* texture, bool streamed) : m_texture(texture), m_streamed(streamed) {}
StreamedTexture::~StreamedTexture() {
	if (m_streamed) delete m_texture;
}
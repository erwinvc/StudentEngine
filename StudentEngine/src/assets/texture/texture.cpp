#include "stdafx.h"

Texture::Texture(const String& name, int32 width, int32 height, byte* data, TextureParameters params, bool keepData) : AssetBase(name), m_params(params), m_textureID(0), m_width(width), m_height(height), m_path(""), m_data(nullptr), m_keepData(keepData) {
	SetData(data);
}

Texture::Texture(const String& name, int32 width, int32 height, TextureParameters params, bool keepData) : AssetBase(name), m_params(params), m_textureID(0), m_width(width), m_height(height), m_path(""), m_data(nullptr), m_keepData(keepData) {
	SetData(nullptr);
}

void Texture::SetData(byte* data) {
	GL(glGenTextures(1, &m_textureID));

	SetTextureParameters(m_params);
	GL(glTexImage2D(GL_TEXTURE_2D, 0, m_params.GetInternalFormat(), m_width, m_height, 0, m_params.GetFormat(), m_params.GetType(), data));

	int size = m_width * m_height * m_params.GetChannelCount();

	if (m_keepData && data != nullptr) {
		m_data = new byte[size];
		memcpy(m_data, data, size);
	}
}

void Texture::SetTextureParameters(TextureParameters& params)
{
	m_params = params;
	GL(glBindTexture(GL_TEXTURE_2D, m_textureID));

	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_params.GetFilter(GL_TEXTURE_MIN_FILTER)));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_params.GetFilter(GL_TEXTURE_MAG_FILTER)));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_params.GetWrap()));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_params.GetWrap()));
}

Texture::~Texture() {
	Cleanup();
}

void Texture::Bind(uint slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::Unbind(uint slot) const {
	glBindTexture(GL_TEXTURE_2D, slot);
}

void Texture::Cleanup() {
	glDeleteTextures(1, &m_textureID);
	if (m_keepData) delete[] m_data;
}

void Texture::Resize(int width, int height) {
	if (m_width == width && m_height == height) return;
	m_width = width;
	m_height = height;

	Bind();
	GL(glTexImage2D(GL_TEXTURE_2D, 0, m_params.GetInternalFormat(), m_width, m_height, 0, m_params.GetFormat(), m_params.GetType(), nullptr));
}
#pragma once

class TextureLoadJob;

class StreamedTexture : public AssetBase {
private:
	Texture* m_texture;
	bool m_streamed;

	void FinishStreaming(Texture* texture) {
		if (m_streamed) return;
		m_texture = texture;
		m_streamed = true;
	}
	friend TextureLoadJob;
public:
	StreamedTexture();
	StreamedTexture(Texture* texture, bool streamed = false);
	~StreamedTexture();

	Texture* GetTexture() { return m_texture; }
	
	operator Texture*() { return m_texture; }
	operator Texture*() const { return m_texture; }
};
#pragma once

class Texture : public AssetBase {
private:
	TextureParameters m_params;
	uint m_textureID;
	int32 m_width, m_height;
	String m_path;
	byte* m_data = nullptr;
	bool m_keepData = false;
	void Cleanup();

public:
	Texture(const String& name, int32 width, int32 height, byte* data, TextureParameters params = TextureParameters(), bool keepData = false);
	Texture(const String& name, int32 width, int32 height, TextureParameters params = TextureParameters(), bool keepData = false);
	//Texture(const String& path, bool hasMipmaps, TextureParameters params = TextureParameters(), bool keepData = false);
	~Texture();

	void SetTextureParameters(TextureParameters& params);
	void SetData(byte* data);
	uint GetHandle() { return m_textureID; }
	void* GetImGuiHandle() { return (void*)(uint64)m_textureID; }
	void Bind(uint slot = 0) const;
	void Unbind(uint slot = 0) const;
	void Resize(int width, int height);

	inline const int32 GetWidth() const { return m_width; }
	inline const int32 GetHeight() const { return m_height; }
	inline byte* GetData() const { return m_data; }
	inline const TextureParameters& GetTextureParams() { return m_params; }
	inline Vector2 GetAspect() {
		float w = GetWidth();
		float h = GetHeight();
		Vector2 aspect(1.0f, 1.0f);
		if (w < h) {
			aspect.x *= w / h;
		} else if (h < w) {
			aspect.y *= h / w;
		}
		return aspect;
	}
	static const String GetAssetTypeName() { return "Texture"; }
};
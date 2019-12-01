#pragma once

class RenderingPipeline {
private:
	bool m_initialized = false;
	bool m_wireframe = false;
	bool m_FXAA = true;

	AssetRef<SpriteRenderer> m_spriteRenderer;
	AssetRef<FrameBuffer> m_finalFBO;
	AssetRef<Texture> m_finalTexture;

public:
	RenderingPipeline() {}
	~RenderingPipeline();
	AssetRef<OrthographicCamera> m_camera;

	Texture* GetFinalTexture() const { return m_finalTexture; }
	SpriteRenderer* GetSpriteRenderer() const { return m_spriteRenderer; }
	void Initialize();
	void Update(const TimeStep time);
	void Begin();
	void End();
	void OnImGui();

	inline void Rect(float x, float y, float w, float h, float rotation, const Color& color = Color::White(), const Texture* texture = nullptr) {
		m_spriteRenderer->Rect(x, y, w, h, rotation, color, texture);
	}

	inline void Line(float x0, float y0, float x1, float y1, Color& color = Color::White(), float size = 1.0f) {
		m_spriteRenderer->Line(x0, y0, x1, y1, color, size);
	}
};
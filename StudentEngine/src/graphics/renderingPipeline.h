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
	RenderingPipeline(){}
	~RenderingPipeline();
	AssetRef<OrthographicCamera> m_camera;

	Texture* GetFinalTexture() const { return m_finalTexture; }
	SpriteRenderer* GetSpriteRenderer() const { return m_spriteRenderer; }
	void Initialize();
	void Update(const TimeStep time);
	void Begin();
	void End();
	void OnImGui();

	inline void Rect(float x, float y, float w, float h, const Color& color, const Texture* texture = nullptr) {
		m_spriteRenderer->Rect(x, y, w, h, color, texture);
	}
};
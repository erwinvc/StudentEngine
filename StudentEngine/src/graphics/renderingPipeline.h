#pragma once

class RenderingPipeline : public Singleton<RenderingPipeline> {
protected:
	RenderingPipeline() {}
	~RenderingPipeline();
	friend Singleton;

private:
	bool m_initialized = false;
	bool m_wireframe = false;
	bool m_FXAA = true;

	AssetRef<SpriteRenderer> m_spriteRenderer;
	AssetRef<FrameBuffer> m_finalFBO;
	AssetRef<Texture> m_finalTexture;

public:
	AssetRef<OrthographicCamera> m_camera;

	Texture* GetFinalTexture() const { return m_finalTexture; }
	SpriteRenderer* GetSpriteRenderer() const { return m_spriteRenderer; }
	void Initialize();
	void Update(const TimeStep time);
	void Begin();
	void End();
	void OnImGui();
};

inline RenderingPipeline* GetPipeline() { return RenderingPipeline::GetInstance(); }

namespace Draw {
	void Rect(float x, float y, float w, float h, const Color& color) { GetPipeline()->GetSpriteRenderer()->Rect(x, y, w, h, color); }
}
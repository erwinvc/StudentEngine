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
	//	SpriteRenderer* m_spriteRenderer;

	AssetRef<OrthographicCamera> m_camera;
	
	AssetRef<Texture> GetFinalTexture() const { return m_finalTexture; }
	void Initialize();
	void Update(const TimeStep time);
	void Draw();
};
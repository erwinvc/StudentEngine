#pragma once

class RenderingPipeline {
private:
	bool m_initialized = false;
	bool m_wireframe = false;
	bool m_FXAA = true;

	Camera* m_mainCamera;
	Camera* m_activeCamera;
	Mesh* m_quad;
	SpriteRenderer* m_spriteRenderer;
	FrameBuffer* m_finalFBO;
	Texture* m_finalTexture;

public:
	RenderingPipeline() {}
	~RenderingPipeline();

	Texture* GetFinalTexture() const { return m_finalTexture; }
	SpriteRenderer* GetSpriteRenderer() const { return m_spriteRenderer; }
	void Initialize();
	void Update(const TimeStep time);
	void Begin();
	void EndSpriteRenderer();
	void Finish();
	void OnImGui();

	float Width() { return m_activeCamera->GetRelativeViewport().z; }
	float Height() { return m_activeCamera->GetRelativeViewport().w; }

	bool Initialized() { return m_initialized; }

	Camera* GetCamera() { return m_activeCamera; }
	void SetCamera(Camera* camera) { m_activeCamera = camera; }

	Mesh* GetQuad() { return m_quad; }

	inline void LineRect(Rectangle& rect, Color& color = Color::White(), float lineSize = 1.0f) {
		m_spriteRenderer->LineRect(rect, color, lineSize * m_activeCamera->GetZoom());
	}
	
	inline void Rect(Rectangle& rect, float rotation = 0, const Color& color = Color::White(), const StreamedTexture* texture = nullptr, const Vector3 atlasValues = Vector3(1, 0, 0)) {
		m_spriteRenderer->Rect(rect.x, rect.y, rect.w, rect.h, rotation, color, texture, atlasValues);
	}
	
	inline void Rect(float x, float y, float w, float h, float rotation = 0, const Color& color = Color::White(), const StreamedTexture* texture = nullptr, const Vector3 atlasValues = Vector3(1, 0, 0)) {
		m_spriteRenderer->Rect(x, y, w, h, rotation, color, texture, atlasValues);
	}

	inline void Rect(float x, float y, float w, float h, float uMin, float uMax, float vMin, float vMax, float rotation = 0, const Color& color = Color::White(), const StreamedTexture* texture = nullptr, const Vector3 atlasValues = Vector3(1, 0, 0)) {
		m_spriteRenderer->Rect(x, y, w, h, uMin, uMax, vMin, vMax, rotation, color, texture, atlasValues);

	}
	inline void Line(float x0, float y0, float x1, float y1, Color& color = Color::White(), float size = 1.0f) {
		m_spriteRenderer->Line(x0, y0, x1, y1, color, size * m_activeCamera->GetZoom());
	}
};
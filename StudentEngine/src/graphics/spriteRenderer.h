#pragma once

class SpriteRenderer {
public:
	struct Vertex {
		Vector3 m_position;
		Vector2 m_uv;
		float m_textureID;
		float m_materialID;
		float m_color;
	};

	const int MAX_SPRITES = 60000;
	const int SPRITE_SIZE = sizeof(Vertex) * 4;
	const int BUFFER_SIZE = SPRITE_SIZE * MAX_SPRITES;
	const int INDICES_SIZE = MAX_SPRITES * 6;
	const int MAX_TEXTURES = 32 - 1;

	BufferLayout m_layout = {
		{VertexBufferDataType::Float3, "vsPos", 0},
		{VertexBufferDataType::Float2, "vsUv", 0},
		{VertexBufferDataType::Float, "vsTextureID", 0},
		{VertexBufferDataType::Float, "vsMaterialID", 0},
		{VertexBufferDataType::Float, "vsColor", 0}
	};

	Shader* m_shader;

	InstancedRenderer<Vertex>* m_instancedRenderer;

	SpriteRenderer() {
		uint indices[] = { 0, 1, 2, 0, 2, 3 };
		Mesh* mesh = new Mesh(new VertexArray(), new IndexBuffer(indices, 6));
		m_instancedRenderer = new InstancedRenderer<Vertex>(mesh, MAX_SPRITES, m_layout);
		delete mesh;

		m_shader = GetShaderManager()->Create("Sprite", "res/shaders/sprite");
	}
	~SpriteRenderer() {
		delete m_instancedRenderer;
	}

	void Submit(const Sprite& sprite)
	{
	}

	void Begin() {
		m_instancedRenderer->Begin();
	}

	void End() {
		m_instancedRenderer->End();
	}

	void Draw() {
		m_instancedRenderer->Draw();
	}
};
#pragma once

class RenderingPipeline;

class SpriteRenderer {
public:
	struct Vertex {
		Vector3 m_position;
		Vector2 m_uv;
		float m_textureID;
		Color m_color;
		Vector3 vsAtlasValues;
	};

	vector<const Texture*> m_textures;
	const int MAX_SPRITES = 50000;
	const int SPRITE_SIZE = sizeof(Vertex) * 4;
	const int BUFFER_SIZE = SPRITE_SIZE * MAX_SPRITES;
	const int INDICES_SIZE = MAX_SPRITES * 6;
	const int MAX_TEXTURES = 32 - 1;

	BufferLayout m_layout = {
		{VertexBufferDataType::Float3, "vsPos", 0},
		{VertexBufferDataType::Float2, "vsUv", 0},
		{VertexBufferDataType::Float, "vsTextureID", 0},
		{VertexBufferDataType::Float4, "vsColor", 0},
		{VertexBufferDataType::Float3, "vsAtlasValues", 0}
	};

	Shader* m_shader;

	InstancedRenderer<Vertex>* m_instancedRenderer;
	Texture* m_defaultTexture;

	SpriteRenderer(Texture* defaultTexture) : m_defaultTexture(defaultTexture) {
		uint32* indicesBuffer = new uint32[INDICES_SIZE];

		//Hardcoded
		uint32 offset = 0;
		for (int32 i = 0; i < MAX_SPRITES; i += 6) {
			indicesBuffer[i] = offset + 0;
			indicesBuffer[i + 1] = offset + 1;
			indicesBuffer[i + 2] = offset + 2;

			indicesBuffer[i + 3] = offset + 2;
			indicesBuffer[i + 4] = offset + 3;
			indicesBuffer[i + 5] = offset + 0;

			offset += 4;
		}

		Mesh* mesh = new Mesh(new VertexArray(), new IndexBuffer(indicesBuffer, INDICES_SIZE));
		m_instancedRenderer = new InstancedRenderer<Vertex>(mesh, MAX_SPRITES, 6, m_layout);
		delete mesh;
		delete[] indicesBuffer;
		m_shader = GetShaderManager()->Get("Sprite");
		m_shader->Bind();

		uint32 textures[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 ,16, 17, 18, 19, 20, 21, 22, 23 ,24, 25, 26, 27, 28, 29, 30, 31 };
		m_shader->Set("_Textures", textures, 32);
	}

	~SpriteRenderer() {
		delete m_instancedRenderer;
	}

	void Rect(float x, float y, float w, float h, float rotation, const Color& color, const StreamedTexture* texture = nullptr, const Vector3& atlasValues = Vector3(1, 0, 0)) {
		float textureSlot = 0.0f;
		if (texture) textureSlot = SubmitTexture(*texture);
		Vertex vertices[4] = { 0 };
		Matrix4 rot = Matrix4::Rotate(rotation, Vector3::ZAxis());
		//Top left
		vertices[0].m_position = Vector3(x, y, 0) + rot.Multiply(Vector3(-w / 2, h / 2));
		vertices[0].m_uv = Vector2(0.0f, 1.0f);
		vertices[0].m_textureID = textureSlot;
		vertices[0].m_color = color;
		vertices[0].vsAtlasValues = atlasValues;
		//Top right
		vertices[1].m_position = Vector3(x, y, 0) + rot.Multiply(Vector3(w / 2, h / 2));
		vertices[1].m_uv = Vector2(1.0f, 1.0f);
		vertices[1].m_textureID = textureSlot;
		vertices[1].m_color = color;
		vertices[1].vsAtlasValues = atlasValues;
		//Bottom right
		vertices[2].m_position = Vector3(x, y, 0) + rot.Multiply(Vector3(w / 2, -h / 2));
		vertices[2].m_uv = Vector2(1.0f, 0.0f);
		vertices[2].m_textureID = textureSlot;
		vertices[2].m_color = color;
		vertices[2].vsAtlasValues = atlasValues;
		//Bottom left
		vertices[3].m_position = Vector3(x, y, 0) + rot.Multiply(Vector3(-w / 2, -h / 2));
		vertices[3].m_uv = Vector2(0.0f, 0.0f);
		vertices[3].m_textureID = textureSlot;
		vertices[3].m_color = color;
		vertices[3].vsAtlasValues = atlasValues;

		m_instancedRenderer->Submit(vertices);
	}

	void Rect(float x, float y, float w, float h, float uMin, float uMax, float vMin, float vMax, float rotation = 0, const Color& color = Color::White(), const StreamedTexture* texture = nullptr, const Vector3& atlasValues = Vector3(1, 0, 0)) {
		float textureSlot = 0.0f;
		if (texture) textureSlot = SubmitTexture(*texture);
		Vertex vertices[4] = { 0 };
		Matrix4 rot = Matrix4::Rotate(rotation, Vector3::ZAxis());
		//Top left
		vertices[0].m_position = Vector3(x, y, 0) + rot.Multiply(Vector3(-w / 2, h / 2));
		vertices[0].m_uv = Vector2(uMin, vMax);
		vertices[0].m_textureID = textureSlot;
		vertices[0].m_color = color;
		vertices[0].vsAtlasValues = atlasValues;
		//Top right
		vertices[1].m_position = Vector3(x, y, 0) + rot.Multiply(Vector3(w / 2, h / 2));
		vertices[1].m_uv = Vector2(uMax, vMax);
		vertices[1].m_textureID = textureSlot;
		vertices[1].m_color = color;
		vertices[1].vsAtlasValues = atlasValues;
		//Bottom right
		vertices[2].m_position = Vector3(x, y, 0) + rot.Multiply(Vector3(w / 2, -h / 2));
		vertices[2].m_uv = Vector2(uMax, vMin);
		vertices[2].m_textureID = textureSlot;
		vertices[2].m_color = color;
		vertices[2].vsAtlasValues = atlasValues;
		//Bottom left
		vertices[3].m_position = Vector3(x, y, 0) + rot.Multiply(Vector3(-w / 2, -h / 2));
		vertices[3].m_uv = Vector2(uMin, vMin);
		vertices[3].m_textureID = textureSlot;
		vertices[3].m_color = color;
		vertices[3].vsAtlasValues = atlasValues;

		m_instancedRenderer->Submit(vertices);
	}
	void Line(float x0, float y0, float x1, float y1, Color& color = Color::White(), float size = 1.0f) {
		float textureSlot = 0.0f;
		Vector2 normal = Vector2(y1 - y0, -(x1 - x0)).Normalize() * size;
		Vertex vertices[4] = { 0 };

		vertices[0].m_position = Vector3(x0 + normal.x, y0 + normal.y, 0.0f);
		vertices[0].m_uv = Vector2(0.0f, 1.0f);
		vertices[0].m_textureID = textureSlot;
		vertices[0].m_color = color;
		vertices[0].vsAtlasValues = Vector3(1, 0, 0);

		vertices[1].m_position = Vector3(x1 + normal.x, y1 + normal.y, 0.0f);
		vertices[1].m_uv = Vector2(1.0f, 1.0f);
		vertices[1].m_textureID = textureSlot;
		vertices[1].m_color = color;
		vertices[1].vsAtlasValues = Vector3(1, 0, 0);

		vertices[2].m_position = Vector3(x1 - normal.x, y1 - normal.y, 0.0f);
		vertices[2].m_uv = Vector2(1.0f, 0.0f);
		vertices[2].m_textureID = textureSlot;
		vertices[2].m_color = color;
		vertices[2].vsAtlasValues = Vector3(1, 0, 0);
		
		vertices[3].m_position = Vector3(x0 - normal.x, y0 - normal.y, 0.0f);
		vertices[3].m_uv = Vector2(0.0f, 0.0f);
		vertices[3].m_textureID = textureSlot;
		vertices[3].m_color = color;
		vertices[3].vsAtlasValues = Vector3(1, 0, 0);

		m_instancedRenderer->Submit(vertices);
	}

	void LineRect(Rectangle& rect, Color& color = Color::White(), float lineSize = 1.0f) {
		float halfLineSize = lineSize / 2;
		Line(rect.XMin() - halfLineSize, rect.YMin() - halfLineSize, rect.XMin() - halfLineSize, rect.YMax() + halfLineSize, color, lineSize);
		Line(rect.XMax() + halfLineSize, rect.YMin() - halfLineSize, rect.XMax() + halfLineSize, rect.YMax() + halfLineSize, color, lineSize);
		Line(rect.XMin() - halfLineSize, rect.YMin() - halfLineSize, rect.XMax() + halfLineSize, rect.YMin() - halfLineSize, color, lineSize);
		Line(rect.XMin() - halfLineSize, rect.YMax() + halfLineSize, rect.XMax() + halfLineSize, rect.YMax() + halfLineSize, color, lineSize);
	}

	void Begin() {
		m_textures.clear();
		SubmitTexture(m_defaultTexture);
		m_instancedRenderer->Begin();
	}

	void End() {
		m_instancedRenderer->End();
	}

	void Draw(RenderingPipeline* pipeline);
private:
	float SubmitTexture(const Texture* texture) {
		float result = 0.0f;
		bool found = false;
		for (uint i = 0; i < m_textures.size(); i++) {
			if (m_textures[i] == texture) {
				result = (float)(i);
				found = true;
				break;
			}
		}

		if (!found) {
			if (m_textures.size() >= 31) {
				//#TODO: Flush
			}
			m_textures.push_back(texture);
			result = (float)(m_textures.size() - 1);
		}
		return result;
	}
};
#pragma once

class RenderingPipeline;
class Sprite {
public:
	StreamedTexture* m_singleFrameTexture = nullptr;
	StreamedTexture* m_texture = nullptr;
	Color m_color = Color::White();
	Vector3 m_atlasValues = Vector3(1, 0, 0);
	bool m_9Slice = false;
	int m_numberOfRows = 1;
	int m_frameCount = 0;
	int m_frameOffset = 0;
	float m_frameTime = 0;

	void InspectorDraw(GameObject* gameObject) {
		if (InspectorDrawer::Header("Sprite")) {
			InspectorDrawer::Texture(gameObject, "Texture", m_texture);
			InspectorDrawer::Color(gameObject, "Color", m_color);
			InspectorDrawer::Vec3(gameObject, "Atlas Values", m_atlasValues);
		}
	}

	bool Compare(const Sprite& other) {
		return m_color == other.m_color && m_texture->GetTexture()->GetHandle() == other.m_texture->GetTexture()->GetHandle();
	}

	virtual void Draw(RenderingPipeline* pipeline, const Transform& transform);

	friend void to_json(nlohmann::json& j, const Sprite& obj);
	friend void from_json(const nlohmann::json& j, Sprite& obj);
};
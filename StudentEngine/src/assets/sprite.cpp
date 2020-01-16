#include "stdafx.h"

void Sprite::Draw(RenderingPipeline* pipeline, const Transform& transform) {
	StreamedTexture*& tex = m_singleFrameTexture ? m_singleFrameTexture : m_texture;
	if (m_9Slice) {
		Vector2 pos = transform.m_position;
		Vector2 size = transform.m_size;
		Vector2 halfPos = pos / 2.0f;
		Vector2 halfSize = size / 2.0f;

		float partSize = 1.0f / 3.0f;
		float twoPartSize = partSize * 2.0f;
		float slSize = 16.0f; //Slice size
		float halfSlSize = slSize / 2.0f;
		float twoSlSize = slSize * 2.0f;

		int yCount = int(size.y / slSize);
		int xCount = int(size.x / slSize);
		float yDifference = size.y / slSize - yCount;
		float xDifference = size.x / slSize - xCount;
		for (int i = 1; i < yCount - 1; i++) {
			pipeline->Rect(pos.x - halfSize.x + halfSlSize, ((i + 0.5f) * slSize) - (size.y / 2) + pos.y, slSize, slSize, 0.0f, partSize, partSize, 1.0f - partSize, 0.0f, m_color, tex); // Left
			pipeline->Rect(pos.x + halfSize.x - halfSlSize, ((i + 0.5f) * slSize) - (size.y / 2) + pos.y, slSize, slSize, 1.0f - partSize, 1.0f, partSize, 1.0f - partSize, 0.0f, m_color, tex); // Right
		}
		if (yDifference > 0) {
			pipeline->Rect(pos.x - halfSize.x + halfSlSize, ((yCount - 1.0f) * slSize) - (size.y / 2) + pos.y + (slSize * yDifference) / 2, slSize, slSize * yDifference, 0.0f, partSize, partSize, partSize + ((1.0f - twoPartSize) * yDifference), 0.0f, m_color, tex); // Left
			pipeline->Rect(pos.x + halfSize.x - halfSlSize, ((yCount - 1.0f) * slSize) - (size.y / 2) + pos.y + (slSize * yDifference) / 2, slSize, slSize * yDifference, 1.0f - partSize, 1.0f, partSize, partSize + ((1.0f - twoPartSize) * yDifference), 0.0f, m_color, tex); // Right
		}

		for (int i = 1; i < xCount - 1; i++) {
			pipeline->Rect(((i + 0.5f) * slSize) - (size.x / 2) + pos.x, pos.y - halfSize.y + halfSlSize, slSize, slSize, partSize, 1.0f - partSize, 0.0f, partSize, 0.0f, m_color, tex); //Bottom
			pipeline->Rect(((i + 0.5f) * slSize) - (size.x / 2) + pos.x, pos.y + halfSize.y - halfSlSize, slSize, slSize, partSize, 1.0f - partSize, 1.0f - partSize, 1.0f, 0.0f, m_color, tex); //Top
		}

		if (xDifference > 0) {
			pipeline->Rect(((xCount - 1.0f) * slSize) - (size.x / 2) + pos.x + (slSize * xDifference) / 2, pos.y - halfSize.y + halfSlSize, slSize * xDifference, slSize, partSize, partSize + (1.0f - twoPartSize) * xDifference, 0.0f, partSize, 0.0f, m_color, tex); //Bottom
			pipeline->Rect(((xCount - 1.0f) * slSize) - (size.x / 2) + pos.x + (slSize * xDifference) / 2, pos.y + halfSize.y - halfSlSize, slSize * xDifference, slSize, partSize, partSize + (1.0f - twoPartSize) * xDifference, 1.0f - partSize, 1.0f, 0.0f, m_color, tex); //Top
		}

		pipeline->Rect(pos.x - halfSize.x + halfSlSize, pos.y + halfSize.y - halfSlSize, slSize, slSize, 0.0f, partSize, 1.0f - partSize, 1.0f, 0.0f, m_color, tex); // TopLeft
		pipeline->Rect(pos.x - halfSize.x + halfSlSize, pos.y - halfSize.y + halfSlSize, slSize, slSize, 0.0f, partSize, 0.0f, partSize, 0.0f, m_color, tex); // BottomLeft

		pipeline->Rect(pos.x + halfSize.x - halfSlSize, pos.y + halfSize.y - halfSlSize, slSize, slSize, 1.0f - partSize, 1.0f, 1.0f - partSize, 1.0f, 0.0f, m_color, tex); // TopRight
		pipeline->Rect(pos.x + halfSize.x - halfSlSize, pos.y - halfSize.y + halfSlSize, slSize, slSize, 1.0f - partSize, 1.0f, 0.0f, partSize, 0.0f, m_color, tex); // BottomRight

		pipeline->Rect(pos.x, pos.y, size.x - twoSlSize, size.y - twoSlSize, partSize, 1.0f - partSize, partSize, 1.0f - partSize, 0.0f, m_color, tex);
	} else {
		if (m_frameCount > 0) {
			int textureIndex;
			if (GetStateManager()->GetState() == States::PLAY) {
				textureIndex = (int)(GetApp()->GetTotalFrameTime() / (1000.0f * m_frameTime)) % m_frameCount;
				textureIndex += m_frameOffset;
			} else {
				textureIndex = m_frameOffset;
			}
			float row = m_numberOfRows - (textureIndex / m_numberOfRows) - 1;
			float column = textureIndex % m_numberOfRows;
			m_atlasValues = Vector3(m_numberOfRows, column / m_numberOfRows, row / m_numberOfRows);
		}
		pipeline->Rect(transform.m_position.x, transform.m_position.y, transform.m_size.x, transform.m_size.y, transform.m_rotation, m_color, tex, m_atlasValues);
		if (m_singleFrameTexture) m_singleFrameTexture = nullptr;
	}
}

void to_json(nlohmann::json& j, const Sprite& obj) {
	j = nlohmann::json{ {"texture", obj.m_texture->GetName()}, {"color", obj.m_color}, {"atlas values", obj.m_atlasValues}, {"9slice", obj.m_9Slice}, {"number of rows", obj.m_numberOfRows}, {"frame count", obj.m_frameCount}, {"frame offset", obj.m_frameOffset}, {"frame time", obj.m_frameTime} };
}

void from_json(const nlohmann::json& j, Sprite& obj) {

	obj.m_texture = GetAssetManager()->Get<StreamedTexture>(j.at("texture").get<string>());

	j.at("color").get_to(obj.m_color);
	j.at("atlas values").get_to(obj.m_atlasValues);
	j.at("9slice").get_to(obj.m_9Slice);
	j.at("number of rows").get_to(obj.m_numberOfRows);
	j.at("frame count").get_to(obj.m_frameCount);
	j.at("frame offset").get_to(obj.m_frameOffset);
	j.at("frame time").get_to(obj.m_frameTime);
}
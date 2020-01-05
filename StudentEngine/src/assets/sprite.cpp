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
			pipeline->Rect(pos.x - halfSize.x + halfSlSize, ((i + 0.5f) * slSize) - (size.y / 2) + pos.y, slSize, slSize, 0.0f, partSize, partSize, 1.0f - partSize, 0.0f, Color::White(), tex); // Left
			pipeline->Rect(pos.x + halfSize.x - halfSlSize, ((i + 0.5f) * slSize) - (size.y / 2) + pos.y, slSize, slSize, 1.0f - partSize, 1.0f, partSize, 1.0f - partSize, 0.0f, Color::White(), tex); // Right
		}
		if (yDifference > 0) {
			pipeline->Rect(pos.x - halfSize.x + halfSlSize, ((yCount - 1.0f) * slSize) - (size.y / 2) + pos.y + (slSize * yDifference) / 2, slSize, slSize * yDifference, 0.0f, partSize, partSize, partSize + ((1.0f - twoPartSize) * yDifference), 0.0f, Color::White(), tex); // Left
			pipeline->Rect(pos.x + halfSize.x - halfSlSize, ((yCount - 1.0f) * slSize) - (size.y / 2) + pos.y + (slSize * yDifference) / 2, slSize, slSize * yDifference, 1.0f - partSize, 1.0f, partSize, partSize + ((1.0f - twoPartSize) * yDifference), 0.0f, Color::White(), tex); // Right
		}

		for (int i = 1; i < xCount - 1; i++) {
			pipeline->Rect(((i + 0.5f) * slSize) - (size.x / 2) + pos.x, pos.y - halfSize.y + halfSlSize, slSize, slSize, partSize, 1.0f - partSize, 0.0f, partSize, 0.0f, Color::White(), tex); //Bottom
			pipeline->Rect(((i + 0.5f) * slSize) - (size.x / 2) + pos.x, pos.y + halfSize.y - halfSlSize, slSize, slSize, partSize, 1.0f - partSize, 1.0f - partSize, 1.0f, 0.0f, Color::White(), tex); //Top
		}

		if (xDifference > 0) {
			pipeline->Rect(((xCount - 1.0f) * slSize) - (size.x / 2) + pos.x + (slSize * xDifference) / 2, pos.y - halfSize.y + halfSlSize, slSize * xDifference, slSize, partSize, partSize + (1.0f - twoPartSize) * xDifference, 0.0f, partSize, 0.0f, Color::White(), tex); //Bottom
			pipeline->Rect(((xCount - 1.0f) * slSize) - (size.x / 2) + pos.x + (slSize * xDifference) / 2, pos.y + halfSize.y - halfSlSize, slSize * xDifference, slSize, partSize, partSize + (1.0f - twoPartSize) * xDifference, 1.0f - partSize, 1.0f, 0.0f, Color::White(), tex); //Top
		}

		pipeline->Rect(pos.x - halfSize.x + halfSlSize, pos.y + halfSize.y - halfSlSize, slSize, slSize, 0.0f, partSize, 1.0f - partSize, 1.0f, 0.0f, Color::White(), tex); // TopLeft
		pipeline->Rect(pos.x - halfSize.x + halfSlSize, pos.y - halfSize.y + halfSlSize, slSize, slSize, 0.0f, partSize, 0.0f, partSize, 0.0f, Color::White(), tex); // BottomLeft

		pipeline->Rect(pos.x + halfSize.x - halfSlSize, pos.y + halfSize.y - halfSlSize, slSize, slSize, 1.0f - partSize, 1.0f, 1.0f - partSize, 1.0f, 0.0f, Color::White(), tex); // TopRight
		pipeline->Rect(pos.x + halfSize.x - halfSlSize, pos.y - halfSize.y + halfSlSize, slSize, slSize, 1.0f - partSize, 1.0f, 0.0f, partSize, 0.0f, Color::White(), tex); // BottomRight

		pipeline->Rect(pos.x, pos.y, size.x - twoSlSize, size.y - twoSlSize, partSize, 1.0f - partSize, partSize, 1.0f - partSize, 0.0f, Color::White(), tex);
	} else {
		if (m_frameCount > 0) {
			int textureIndex = (int)(GetApp()->GetTotalFrameTime() / (1000.0f * m_frameTime)) % m_frameCount;
			textureIndex += m_frameOffset;
			float row = m_numberOfRows - (textureIndex / m_numberOfRows) - 1;
			float column = textureIndex % m_numberOfRows;
			m_atlasValues = Vector3(m_numberOfRows, column / m_numberOfRows, row / m_numberOfRows);
		}
		pipeline->Rect(transform.m_position.x, transform.m_position.y, transform.m_size.x, transform.m_size.y, transform.m_rotation, m_color, tex, m_atlasValues);
		if (m_singleFrameTexture) m_singleFrameTexture = nullptr;
	}
}
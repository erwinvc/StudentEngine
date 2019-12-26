#include "stdafx.h"

void Sprite::Draw(RenderingPipeline* pipeline, const Transform& transform) {
	if (m_9Slice) {
		Vector2 pos = transform.m_position;
		Vector2 size = transform.m_size;
		Vector2 halfPos = pos / 2.0f;
		Vector2 halfSize = size / 2.0f;

		float partSize = 1.0f / 3.0f;
		float partSize2 = partSize / 3.0;
		float slSize = 64.0f; //Slice size
		float halfSlSize = slSize / 2.0f;
		float twoSlSize = slSize * 2.0f;

		pipeline->Rect(pos.x - halfSize.x + halfSlSize, pos.y, slSize, size.y - twoSlSize, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, Color::White()); // Left
		pipeline->Rect(pos.x + halfSize.x - halfSlSize, pos.y, slSize, size.y - twoSlSize, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, Color::Red()); // Right
		pipeline->Rect(pos.x, pos.y - halfSize.y + halfSlSize, size.x - twoSlSize, slSize, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, Color::Blue()); //Bottom
		pipeline->Rect(pos.x, pos.y + halfSize.y - halfSlSize, size.x - twoSlSize, slSize, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, Color::Magenta()); //Top

		pipeline->Rect(pos.x - halfSize.x + halfSlSize, pos.y + halfSize.y - halfSlSize, slSize, slSize, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, Color::Yellow()); // TopLeft
		pipeline->Rect(pos.x - halfSize.x + halfSlSize, pos.y - halfSize.y + halfSlSize, slSize, slSize, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, Color::Green()); // BottomLeft

		pipeline->Rect(pos.x + halfSize.x - halfSlSize, pos.y + halfSize.y - halfSlSize, slSize, slSize, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, Color::Cyan()); // TopRight
		pipeline->Rect(pos.x + halfSize.x - halfSlSize, pos.y - halfSize.y + halfSlSize, slSize, slSize, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, Color::NormalMap()); // BottomRight
		
		pipeline->Rect(pos.x, pos.y, size.x - twoSlSize, size.y - twoSlSize, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, Color(0.5f, 1.0f, 0.5f));
	} else {
		pipeline->Rect(transform.m_position.x, transform.m_position.y, transform.m_size.x, transform.m_size.y, transform.m_rotation, m_color, m_singleFrameTexture ? m_singleFrameTexture : m_texture);
		if (m_singleFrameTexture) m_singleFrameTexture = nullptr;
	}
}
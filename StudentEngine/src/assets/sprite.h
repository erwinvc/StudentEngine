#pragma once

class Sprite {
public:
	StreamedTexture* m_singleFrameTexture = nullptr;
	StreamedTexture* m_texture = nullptr;
	Color m_color = Color::White();
};
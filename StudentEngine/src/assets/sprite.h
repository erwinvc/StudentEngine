#pragma once

class Sprite {
public:
	StreamedTexture* m_singleFrameTexture = nullptr;
	StreamedTexture* m_texture = nullptr;
	Color m_color = Color::White();

	void InspectorDraw() {
		if (InspectorDrawer::Header("Sprite")) {
			InspectorDrawer::Texture("Texture", m_texture);
			InspectorDrawer::Color("Color", m_color);
		}
	}
};
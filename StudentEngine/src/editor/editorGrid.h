#pragma once

class EditorGrid {
private:
	static int g_colorValue;
	static Color g_color;
public:
	static float GetGridSizeFromZoomLevel(float zoom) {
		if (zoom < 1.0f) return 8;
		else if (zoom < 5.0f) return 16;
		else if (zoom < 10.0f) return 32;
		else if (zoom < 25.0f) return 64;
		else if (zoom < 50.0f) return 128;
		else if (zoom < 75.0f) return 256;
		else if (zoom < 100.0f) return 512;
		return 1024;
	}
	static void Update(const TimeStep& time) {
		g_color = Color::Mix(Color(1.0f, 1.0f, 1.0f, 0.2f), Color(0.5f, 0.5f, 0.5f, 0.2f), Math::Map(Math::Sin(time.TotalTime() / 1000), -1.0f, 1.0f, 0.0f, 1.0f));
	}
	static void Draw(RenderingPipeline* pipeline) {
		float gridSize = GetGridSizeFromZoomLevel(GetCamera()->GetZoom()) * 2;
		float halfGridSize = gridSize / 2.0f;
		static float lineSize = 0.5f;
		Vector4& vp = GetCamera()->GetRelativeViewport();
		Vector3& pos = GetCamera()->m_position;

		float x = Math::RoundToNumber(pos.x + halfGridSize - (lineSize / 2), gridSize);
		for (; x < pos.x + vp.z; x += gridSize) {
			pipeline->Line(x, pos.y, x, pos.y + vp.w, g_color, lineSize);
		}

		float y = Math::RoundToNumber(pos.y + halfGridSize - (lineSize / 2), gridSize);
		for (; y < pos.y + vp.z; y += gridSize) {
			pipeline->Line(pos.x, y, pos.x + vp.z, y, g_color, lineSize);
		}
	}
};
#pragma once

class EditorGrid {
public:
	static void Draw(RenderingPipeline* pipeline) {
		Vector4& vp = pipeline->m_camera->GetViewport();
		for (int x = 0; x < vp.z; x += 100) {
			//pipeline->Line(x, 0, x, vp.w, Color::White(), 1);
		}
		//for (int y = 0; y < vp.w; y += 20) {
		//}
	}
};
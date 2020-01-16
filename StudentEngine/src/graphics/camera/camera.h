#pragma once

class GameObject;
class Camera {
protected:
	Matrix4 m_projectionMatrix;
	Matrix4 m_viewMatrix;
	Vector4 m_viewPort = Vector4(0, 0, 0, 0);
	float m_zoom = 1.0f;
	bool zoomEnabled = false;

	GameObject* m_target = nullptr;
	Vector2 m_startDiff;
	Vector2 m_deadzone = Vector2(250, 100);

public:
	void UpdateViewMatrix() {
		m_viewMatrix = Matrix4::Translate(-m_position);
	}

	Vector3 m_position = Vector3();

	Camera() {}
	~Camera() {}

	void UpdateZoom(const TimeStep& time) {
		if (zoomEnabled) {
			float oldZoom = m_zoom;
			if (KeyJustDown('F')) {
				m_zoom = 1.0f;
				m_position = Vector2(0, 0);
			}
			float zoom = GetMouse()->GetScroll().y / 10;
			m_zoom = Math::Clamp(m_zoom - zoom, 0.1f, 10.0f);
			m_zoom = Math::RoundToNumber(m_zoom, 0.1f);
			float difference = oldZoom - m_zoom;
			if (difference != 0) {
				m_position += Vector2(m_viewPort.z * difference, m_viewPort.w * difference) / 2;
				UpdateProjectionMatrix();
			}
		}
	}

	// Moved to .cpp to apply forward declaration to GameObject;
	void Update(const TimeStep time);

	void UpdateProjectionMatrix() {
		m_projectionMatrix = Matrix4::Orthographic(0, m_viewPort.z * m_zoom, 0, m_viewPort.w * m_zoom, -1.0f, 1.0f);
	}

	void SetViewport(float x, float y, float width, float height) {
		if (m_viewPort.x == x && m_viewPort.y == y && m_viewPort.z == width && m_viewPort.w == height) return;
		m_viewPort.x = (float)x;
		m_viewPort.y = (float)y;
		m_viewPort.z = (float)width;
		m_viewPort.w = (float)height;
		UpdateProjectionMatrix();
	}

	void SetViewport(Vector4 viewport) {
		SetViewport(viewport.x, viewport.y, viewport.z, viewport.w);
	}

	void SetZoomEnabled(bool toggle) {
		zoomEnabled = toggle;
	}

	float GetZoom() { return m_zoom; }
	void SetZoom(float zoom) { m_zoom = zoom; }

	void SetTarget(GameObject* target);

	inline Matrix4 GetProjectionMatrix() const { return m_projectionMatrix; }
	inline Matrix4 GetViewMatrix() const { return m_viewMatrix; }
	inline Vector4 GetRelativeViewport() const { return Vector4(m_viewPort.x, m_viewPort.y, m_viewPort.z * m_zoom, m_viewPort.w * m_zoom); }
	inline Vector4 GetActualViewport() const { return m_viewPort; }

	virtual void OnImGui() {
		ImGui::InputFloat3("Position", (float*)&m_position);
	}
};
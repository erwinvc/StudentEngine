#pragma once

class Camera {
protected:
	Matrix4 m_projectionMatrix;
	Matrix4 m_viewMatrix;
	Vector4 m_viewPort;
	float m_zoom = 1.0f;

protected:
	void UpdateViewMatrix() {
		m_viewMatrix = Matrix4::Translate(-m_position);
	}

public:
	Vector3 m_position = Vector3();

	Camera() {}
	~Camera() {}
	
	void Update(const TimeStep time) {
		float zoom = GetMouse()->GetScroll().y / 10;
		m_zoom = Math::Clamp(m_zoom - zoom, 0.1f, 100.0f);
		m_position += Vector2(m_viewPort.z * zoom, m_viewPort.w * zoom) / 2;
		UpdateViewMatrix();
	}

	void UpdateProjectionMatrix() {
		m_projectionMatrix = Matrix4::Orthographic(0, m_viewPort.z * m_zoom, 0, m_viewPort.w * m_zoom, -1.0f, 1.0f);
	}

	void SetViewport(uint x, uint y, uint width, uint height) {
		m_viewPort.x = (float)x;
		m_viewPort.y = (float)y;
		m_viewPort.z = (float)width;
		m_viewPort.w = (float)height;
		UpdateProjectionMatrix();
	}

	float GetZoom() { return m_zoom; }

	inline Matrix4 GetProjectionMatrix() const { return m_projectionMatrix; }
	inline Matrix4 GetViewMatrix() const { return m_viewMatrix; }
	inline Vector4 GetRelativeViewport() const { return Vector4(m_viewPort.x, m_viewPort.y, m_viewPort.z * m_zoom, m_viewPort.w * m_zoom); }
	inline Vector4 GetActualViewport() const { return m_viewPort; }

	virtual void OnImGui() {
		ImGui::InputFloat3("Position", (float*)&m_position);
	}
};
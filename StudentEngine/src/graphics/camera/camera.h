#pragma once

class Camera {
protected:
	Matrix4 m_projectionMatrix;
	Matrix4 m_viewMatrix;
	Vector4 m_viewPort;

protected:
	void UpdateViewMatrix() {
		Matrix4 transform = Matrix4::Translate(Vector3(0, 0, 1));
		transform *= Matrix4::Rotate(m_rotation.x, Vector3::XAxis());
		transform *= Matrix4::Rotate(m_rotation.y, Vector3::YAxis());
		transform *= Matrix4::Rotate(m_rotation.z, Vector3::ZAxis());
		transform *= Matrix4::Translate(-m_position);
		m_viewMatrix = transform;
	}

public:
	Vector3 m_position = Vector3();
	Vector3 m_rotation = Vector3();

	Camera() { }

	virtual ~Camera() {}
	virtual void Update(const TimeStep time) {}

	virtual void UpdateProjectionMatrix() = 0;

	void SetViewport(uint x, uint y, uint width, uint height) {
		m_viewPort.x = (float)x;
		m_viewPort.y = (float)y;
		m_viewPort.z = (float)width;
		m_viewPort.w = (float)height;
		UpdateProjectionMatrix();
	}

	inline Matrix4 GetProjectionMatrix() const { return m_projectionMatrix; }
	inline Matrix4 GetViewMatrix() const { return m_viewMatrix; }
	inline Vector4 GetViewport() const { return m_viewPort; }

	virtual void OnImGui() {
		ImGui::InputFloat3("Position", (float*)&m_position);
		ImGui::InputFloat3("Rotation", (float*)&m_rotation);
	}
};
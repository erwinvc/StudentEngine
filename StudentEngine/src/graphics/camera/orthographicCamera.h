#pragma once

class OrthographicCamera : public Camera {
private:
	float m_left;
	float m_right;
	float m_top;
	float m_bottom;
	float m_near;
	float m_far;
public:
	OrthographicCamera(){}
	OrthographicCamera(float left, float right, float top, float bottom, float near, float far) :
	m_left(left), m_right(right), m_top(top), m_bottom(bottom), m_near(near), m_far(far), Camera() {}
	~OrthographicCamera() {}
	void Update(const TimeStep time) {
		UpdateViewMatrix();
	}

	void UpdateProjectionMatrix() {
		//m_projectionMatrix = Matrix4::Orthographic(m_left, m_right, m_bottom, m_top, m_near, m_far);
		m_projectionMatrix = Matrix4::Orthographic(0, m_viewPort.z, 0, m_viewPort.w, -1.0f, 1.0f);
	}
};
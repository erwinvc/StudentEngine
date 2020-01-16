#include "stdafx.h"

static Mouse g_mouse;

void Mouse::Initialize(Window* window) {
    for (int i = 0; i < BUTTONSSIZE; i++) {
        m_buttonStates[i] = Button();
    }

    GetGLCallbackManager()->AddOnMouseButtonCallback(this, &Mouse::OnMouseButton);
    GetGLCallbackManager()->AddOnMousePosCallback(this, &Mouse::OnMousePos);
    GetGLCallbackManager()->AddOnScrollCallback(this, &Mouse::OnScroll);
	LOG("[~cInput~x] Initialized Mouse");
}

void Mouse::OnMouseButton(int button, int action, int mods) {
    if (action == GLFW_RELEASE) {
        m_buttonStates[button].m_glIsUpNow = true;
    } else if (action == GLFW_PRESS) {
        GetMouse()->m_lastButton = button;
       // if (m_buttonStates[button].m_doubleClickTimer + 1000 < glfwGetTime()) m_buttonStates[button].m_doubleClicked = true;
        m_buttonStates[button].m_glDoubleClicked = (m_buttonStates[button].m_glIsUpNow && glfwGetTime() - m_buttonStates[button].m_time < 0.25f);
		m_buttonStates[button].m_glIsUpNow = false;
        m_buttonStates[button].m_time = glfwGetTime();
    }
}

void Mouse::OnMousePos(double xpos, double ypos) {
    GetMouse()->m_position.x = (float)xpos;
    GetMouse()->m_position.y = (float)ypos;
}


void Mouse::OnScroll(double xoffset, double yoffset) {
    GetMouse()->m_scrollBuffer.x += (float)xoffset;
    GetMouse()->m_scrollBuffer.y += (float)yoffset;
}
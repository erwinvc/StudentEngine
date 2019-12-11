#include "stdafx.h"

void GLFiberManager::Initialize() {
	if (m_initialized) return;
	m_glThreadID = this_thread::get_id();
	ConvertThreadToFiber(nullptr);
	m_mainFiber = GetCurrentFiber();
	m_initialized = true;
	LOG("[~rThreads~x] GLFiber created");
}

void GLFiberManager::AddFiber(String name, void(*function)()) {
	uint64* args = new uint64[2]{ (uint64)function, (uint64)m_mainFiber };
	LPVOID fiber = CreateFiber(NULL, [](LPVOID lpFiberParameter) {
		uint64* arguments = (uint64*)lpFiberParameter;
		while (true) {
			((void(*)(void)) arguments[0])();
			SwitchToFiber((LPVOID)arguments[1]);
		}
	}, args);

	Fiber newFiber = { fiber, nullptr, (float)glfwGetTime(), name, args };
	m_fibers.push_back(newFiber);

	for (auto& fiber = m_fibers.begin(); fiber != m_fibers.end(); ++fiber) {
		auto& nextFiber = fiber + 1;
		if (nextFiber != m_fibers.end()) fiber->m_nextFiber = &*nextFiber;
		else fiber->m_nextFiber = &m_fibers.front();
	}
	m_currentFiber = &m_fibers.front();
	LOG("[~rThreads~x] Created ~1%s~x fiber", newFiber.m_name.c_str());
}

void GLFiberManager::Tick() {
	if (!m_currentFiber || !m_initialized) return;
	for (Fiber& f : m_fibers) {
		if (glfwGetTime() >= f.m_wakeTime) {
			SwitchToFiber(f.m_fiber);
		}
	}
	m_currentFiber = m_currentFiber->m_nextFiber;
}

void GLFiberManager::Sleep(float waitTime) {
	auto& fiber = find_if(m_fibers.begin(), m_fibers.end(), [](const Fiber& fiber) -> bool { return fiber.m_fiber == GetCurrentFiber(); });
	if (fiber != m_fibers.end()) {
		fiber->m_wakeTime = (float)glfwGetTime() + waitTime;
		SwitchToFiber(m_mainFiber);
	}
}

void GLFiberManager::GoToMainFiber() {
	SwitchToFiber(m_mainFiber);
}

void GLFiberManager::Delete(String name) {
	for (Fiber& fiber : m_fibers) {
		if (fiber.m_name.compare(name) == 0) {
			DeleteFiber(fiber.m_fiber);
			LOG("[~rThreads~x] Deleted ~1%s~x fiber", fiber.m_name.c_str());
			break;
		}
	}
}

void GLFiberManager::Cleanup() {
	if (!m_cleaned) {
		for (Fiber& fiber : m_fibers) {
			delete[] fiber.m_args;
			DeleteFiber(fiber.m_fiber);
			LOG("[~rThreads~x] Deleted ~1%s~x fiber", fiber.m_name.c_str());
		}
		//LOG_DEBUG("[Threads] %s deleted", m_name);
		m_cleaned = true;
	}
}
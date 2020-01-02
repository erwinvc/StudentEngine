#pragma once

template<typename T>
class AsyncQueue {
private:
	mutex m_lock;
	condition_variable m_conditionVariable;
	queue <T> m_queue;
	bool m_releaseThreads = false;

public:

	template<typename... Args>
	void Emplace(Args&& ... args) {
		m_queue.emplace(T(forward<Args>(args)...));
		m_conditionVariable.notify_one();
	}
	
	void Add(T obj) {
		unique_lock <mutex> l(m_lock);
		m_queue.emplace(std::move(obj));
		m_conditionVariable.notify_one();
	}

	bool TryToGet(T& obj) {
		unique_lock <mutex> lock(m_lock);
		if (m_queue.empty()) return false;
		obj = move(m_queue.front());
		m_queue.pop();
		return true;
	}

	bool Peek(T& obj) {
		unique_lock <mutex> lock(m_lock);
		if (m_queue.empty()) return false;
		obj = move(m_queue.front());
		return true;
	}

	bool WaitForGet(T& obj) {
		unique_lock <mutex> lock(m_lock);
		while (!m_releaseThreads && m_queue.empty()) m_conditionVariable.wait(lock);
		if (m_queue.empty()) return false;
		obj = move(m_queue.front());
		m_queue.pop();
		return true;
	}

	void ReleaseWaitingThreadsAndCleanup() {
		m_releaseThreads = true;
		m_conditionVariable.notify_all();
		queue<T>().swap(m_queue);
	}

	int Size() {
		unique_lock <mutex> lock(m_lock);
		return (int)m_queue.size();
	}
};
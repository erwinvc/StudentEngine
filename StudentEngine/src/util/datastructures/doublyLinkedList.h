#pragma once

template<typename T>
class DoublyLinkedList {
private:
	struct Node {
		Node* m_next;
		Node* m_previous;
		T m_data;
	};

	int m_size = 0;
	Node* m_tail = nullptr;
	Node* m_head = nullptr;


public:
	void Add(T entry) {
		if (!m_head) {
			m_head = new Node();
			m_head->m_data = entry;
			m_head->m_next = nullptr;
			m_head->m_previous = nullptr;
			m_tail = m_head;
		} else {
			Node* node = new Node();
			node->m_data = entry;

			node->m_previous = m_head;
			m_head->m_next = node;
			m_head = node;
		}
		m_size++;
	}

	T& GetFirst() {
		if (!m_tail) throw out_of_range("DoublyLinkedList");
		return m_tail->m_data;
	}

	void RemoveFirst() {
		if (!m_tail) throw out_of_range("DoublyLinkedList");
		Node* temp = m_tail;
		if (m_tail->m_next) {
			m_tail = m_tail->m_next;
		}
		delete temp;
		m_size--;
	}

	T& GetLast() {
		if (!m_head) throw out_of_range("DoublyLinkedList");
		return m_head->m_data;
	}

	void RemoveLast() {
		if (!m_head) throw out_of_range("DoublyLinkedList");
		Node* temp = m_head;
		if (m_head->m_previous) {
			m_head = m_head->m_previous;
		}
		delete temp;
		m_size--;
	}

	T& GetIndex(int index) {
		if (index < 0 || index > m_size) throw out_of_range("DoublyLinkedList");
		Node* node = m_tail;
		for (int i = 0; i < index; i++) {
			if (node) node = node->m_next;
		}
		return node->m_data;
	}

	T& operator[] (int index) {
		return GetIndex(index);
	}

	int Size() {
		return m_size;
	}

	bool Empty() {
		return m_size == 0;
	}
};
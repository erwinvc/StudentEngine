#pragma once


namespace UndoTypes {
	class UndoType {
	public:
		virtual void Register(GameObject* gameObject) = 0;
		virtual UndoType* CheckChanged(GameObject* gameObject) = 0;
		virtual void Undo(GameObject* gameObject) = 0;
	};

	class UndoPosition : public UndoType {
	private:
		Vector2 m_position;
	public:
		void Register(GameObject* gameObject) override {
			m_position = gameObject->m_transform.m_position;
		}
		UndoType* CheckChanged(GameObject* gameObject) override {
			UndoPosition* toReturn = nullptr;
			if (gameObject->m_transform.m_position != m_position) {
				toReturn = new UndoPosition();
				toReturn->m_position = m_position;
			}
			return toReturn;
		}
		void Undo(GameObject* gameObject) override {
			gameObject->m_transform.m_position = m_position;
		}
	};

	class UndoSize : public UndoType {
	private:
		Vector2 m_size;
	public:
		void Register(GameObject* gameObject) override {
			m_size = Vector2(gameObject->m_transform.m_size);
		}
		UndoType* CheckChanged(GameObject* gameObject) override {
			UndoSize* toReturn = nullptr;
			if (gameObject->m_transform.m_size != m_size) {
				toReturn = new UndoSize();
				toReturn->m_size = m_size;
			}
			return toReturn;
		}
		void Undo(GameObject* gameObject) override {
			gameObject->m_transform.m_size = m_size;
		}
	};

	class UndoTexture : public UndoType {
	private:
		Texture* m_texture;
	public:
		void Register(GameObject* gameObject) override {
			m_texture = gameObject->m_sprite.m_texture;
		}
		UndoType* CheckChanged(GameObject* gameObject) override {
			UndoTexture* toReturn = nullptr;
			if (gameObject->m_sprite.m_texture != m_texture) {
				toReturn = new UndoTexture();
				toReturn->m_texture = m_texture;
			}
			return toReturn;
		}
		void Undo(GameObject* gameObject) override {
			gameObject->m_sprite.m_texture = m_texture;
		}
	};

	class UndoColor : public UndoType {
	private:
		Color m_color;
	public:
		void Register(GameObject* gameObject) override {
			m_color = gameObject->m_sprite.m_color;
		}
		UndoType* CheckChanged(GameObject* gameObject) override {
			UndoColor* toReturn = nullptr;
			if (gameObject->m_sprite.m_color != m_color) {
				toReturn = new UndoColor();
				toReturn->m_color = m_color;
			}
			return toReturn;
		}
		void Undo(GameObject* gameObject) override {
			gameObject->m_sprite.m_color = m_color;
		}
	};
};

using namespace UndoTypes;

class Undo {
private:
	static int MAXSTACKSIZE;
	struct UndoEntryCollection {
	public:
		GameObject* m_gameObject;
		vector<UndoType*> m_entries;
		UndoEntryCollection(GameObject* gameObject) : m_gameObject(gameObject) {}
		~UndoEntryCollection() { for (auto& entry : m_entries) delete entry; }
	};

	static GameObject g_baseObject;
	static UndoEntryCollection* m_currentCollection;
	static bool m_recording;
	static DoublyLinkedList<UndoEntryCollection*> m_list;
	static vector<UndoType*> m_types;

public:
	static void Initialize() {
		m_types.push_back(new UndoPosition());
		m_types.push_back(new UndoSize());
	}
	static void Cleanup() {
		for (auto& type : m_types) delete type;
	}
	static void Record(GameObject* gameObject) {
		if (m_recording) {
			LOG_WARN("[Undo] Already recording!");
			return;
		}

		m_recording = true;
		g_baseObject = GameObject(*gameObject);
		m_currentCollection = new UndoEntryCollection(gameObject);
		for (auto& type : m_types) {
			type->Register(gameObject);
		}
	}

	static void FinishRecording() {
		for (auto& type : m_types) {
			UndoType* entry = type->CheckChanged(m_currentCollection->m_gameObject);
			if (entry) m_currentCollection->m_entries.push_back(entry);
		}
		m_list.Add(m_currentCollection);
		if (m_list.Size() > MAXSTACKSIZE) {
			UndoEntryCollection* collection = m_list.GetFirst();
			if (collection) {
				delete collection;
				m_list.RemoveFirst();
			}
		}
		m_recording = false;
	}

	static void UndoOne() {
		if (!m_list.Empty()) {
			UndoEntryCollection* collection = m_list.GetLast();
			for (auto& entry : collection->m_entries) {
				entry->Undo(collection->m_gameObject);
			}
			delete collection;
			m_list.RemoveLast();
		}
	}
};
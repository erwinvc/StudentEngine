#pragma once


namespace UndoTypes {
	class UndoType {
	public:
		virtual void Register(GameObject* gameObject) = 0;
		virtual UndoType* CheckChanged(GameObject* gameObject) = 0;
		virtual void Undo(GameObject* gameObject) = 0;
		virtual void Redo(GameObject* gameObject) = 0;
	};

	class UndoPosition : public UndoType {
	private:
		Vector2 m_position;
		Vector2 m_redoPosition;
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
			m_redoPosition = gameObject->m_transform.m_position;
			gameObject->m_transform.m_position = m_position;
		}

		void Redo(GameObject* gameObject) override {
			gameObject->m_transform.m_position = m_redoPosition;
		}
	};

	class UndoSize : public UndoType {
	private:
		Vector2 m_size;
		Vector2 m_redoSize;
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
			m_redoSize = gameObject->m_transform.m_size;
			gameObject->m_transform.m_size = m_size;
		}
		void Redo(GameObject* gameObject) override {
			gameObject->m_transform.m_size = m_redoSize;
		}
	};

	class UndoTexture : public UndoType {
	private:
		Texture* m_texture;
		Texture* m_redoTexture;
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
			m_redoTexture = gameObject->m_sprite.m_texture;
			gameObject->m_sprite.m_texture = m_texture;
		}

		void Redo(GameObject* gameObject) override {
			gameObject->m_sprite.m_texture = m_redoTexture;
		}
	};

	class UndoColor : public UndoType {
	private:
		Color m_color;
		Color m_redoColor;
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
			m_redoColor = gameObject->m_sprite.m_color;
			gameObject->m_sprite.m_color = m_color;
		}
		void Redo(GameObject* gameObject) override {
			gameObject->m_sprite.m_color = m_redoColor;
		}
	};
};

using namespace UndoTypes;

class Undo {
private:
	static int MAXLISTSIZE;
	static int g_newListIndex;
	static int g_currentListIndex;
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
		m_types.push_back(new UndoTexture());
		m_types.push_back(new UndoColor());
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

		for (int i = 0; i < (m_list.Size() - 1) - g_currentListIndex; i++) {
			delete m_list.GetLast();
			m_list.RemoveLast();
			g_currentListIndex--;
		}

		if (m_list.Size() > MAXLISTSIZE) {
			UndoEntryCollection* collection = m_list.GetFirst();
			if (collection) {
				delete collection;
				m_list.RemoveFirst();
			}
		}
		g_newListIndex = m_list.Size();
		g_currentListIndex = m_list.Size();
		m_recording = false;
	}

	static void UndoOne() {
		if (g_newListIndex > 0) {
			g_newListIndex--;
			GoToNewListIndex();
		}
	}

	static void RedoOne() {
		if (g_newListIndex < m_list.Size()) {
			g_newListIndex++;
			GoToNewListIndex();
		}
	}

	static void GoToNewListIndex() {
		while (g_newListIndex - g_currentListIndex < 0) {
			UndoEntryCollection* collection = m_list[g_currentListIndex - 1];
			for (auto& entry : collection->m_entries) {
				entry->Undo(collection->m_gameObject);
			}
			g_currentListIndex--;
		}
		while (g_newListIndex - g_currentListIndex > 0) {
			UndoEntryCollection* collection = m_list[g_currentListIndex];
			for (auto& entry : collection->m_entries) {
				entry->Redo(collection->m_gameObject);
			}
			g_currentListIndex++;
		}
	}

	static void OnImGui() {
		ImGui::PushItemWidth(ImGui::GetWindowWidth());
		if (ImGui::SliderInt("Undo", &g_newListIndex, 0, m_list.Size())) {
			GoToNewListIndex();
		};
		ImGui::PopItemWidth();
	}
};
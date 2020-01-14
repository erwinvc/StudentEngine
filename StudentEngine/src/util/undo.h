#pragma once

namespace UndoTypes {
	class UndoType {
	public:
		virtual void Register(GameObject* gameObject) = 0;
		virtual UndoType* CheckChanged(GameObject* gameObject) = 0;
		virtual void Undo(GameObject* gameObject) = 0;
		virtual void Redo(GameObject* gameObject) = 0;
		virtual void OnDelete() = 0;
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

		void OnDelete() {}
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
		void OnDelete() {}
	};

	class UndoTexture : public UndoType {
	private:
		StreamedTexture* m_texture;
		StreamedTexture* m_redoTexture;
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
		void OnDelete() {}
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
		void OnDelete() {}
	};

	class UndoObjectDelete : public UndoType {
	private:
		GameObject* m_object;
	public:
		void Register(GameObject* gameObject) override {
			m_object = gameObject;
		}
		UndoType* CheckChanged(GameObject* gameObject) override {
			UndoObjectDelete* toReturn = nullptr;
			if (GetEditorScene()->GetHierarchy().FindObjectByName(gameObject->m_name, false) == nullptr) {
				toReturn = new UndoObjectDelete();
				toReturn->m_object = gameObject;
			}
			return toReturn;
		}
		void Undo(GameObject* gameObject) override {
			GetEditorScene()->AddGameObject(gameObject);
		}
		void Redo(GameObject* gameObject) override {
			GetEditorScene()->DeleteGameObject(gameObject);
		}
		void OnDelete() {
			delete m_object;
		}
	};

	class UndoFolderSwitch : public UndoType {
	private:
		HierarchyObject* m_folder;
		HierarchyObject* m_redoFolder;
		GameObject* m_object;
	public:
		void Register(GameObject* gameObject) override {
			m_object = gameObject;
			m_folder = GetEditorWindow()->FindFolderOfObject(gameObject);
		}
		UndoType* CheckChanged(GameObject* gameObject) override {
			UndoFolderSwitch* toReturn = nullptr;
			if (GetEditorWindow()->FindFolderOfObject(gameObject) != m_folder) {
				toReturn = new UndoFolderSwitch();
				toReturn->m_folder = m_folder;
			}
			return toReturn;
		}
		void Undo(GameObject* gameObject) override {
			m_redoFolder = GetEditorWindow()->FindFolderOfObject(gameObject);
			GetEditorWindow()->MoveToFolder(m_folder, gameObject);
		}
		void Redo(GameObject* gameObject) override {
			GetEditorWindow()->MoveToFolder(m_redoFolder, gameObject);
		}
		void OnDelete() {}
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
		//m_types.push_back(new UndoObjectDelete());
		//m_types.push_back(new UndoFolderSwitch());
		//m_types.push_back(new UndoParent());
		//m_types.push_back(new UndoChild());
		LOG("[~cUndo~x] Initialized Undo");
	}
	static void Cleanup() {
		for (auto& type : m_types) {
			type->OnDelete();
			delete type;
		}
		m_types.clear();
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
		if (!m_recording) return;
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

	static void CancelRecording() {
		if (!m_recording) return;
		delete m_currentCollection;
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

	static bool CanUndo() {
		//if (m_list is nullptr) return false;
		return g_newListIndex > 0;
	}

	static bool CanRedo() {
		//if (m_list == nullptr) return false;
		return g_newListIndex < m_list.Size();
	}

	static void OnImGui() {
		ImGui::PushItemWidth(ImGui::GetWindowWidth());
		if (ImGui::SliderInt("Undo", &g_newListIndex, 0, m_list.Size())) {
			GoToNewListIndex();
		};
		ImGui::PopItemWidth();
	}
};
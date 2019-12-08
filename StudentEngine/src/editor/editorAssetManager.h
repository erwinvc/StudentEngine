#pragma once

enum class EditorAssetType {
	FOLDER,
	SPRITE,
	ANIMATION
};

class EditorAsset {
public:
	String m_name;
	String m_fullName;
	AssetBase* m_asset;
	EditorAssetType m_type;
	EditorAsset* m_parent;

	EditorAsset(const String& name, const String& fullName, AssetBase* asset, EditorAssetType type, EditorAsset* parent) : m_name(name), m_fullName(fullName), m_asset(asset), m_type(type), m_parent(parent) {}

	virtual StreamedTexture* GetTexture();
};

class EditorAssetFolder : public EditorAsset {
public:
	vector<EditorAsset*> m_assets;

	EditorAssetFolder(const String& name, EditorAssetFolder* parent) : EditorAsset(name, "", nullptr, EditorAssetType::FOLDER, parent) {}

	//Checks if first should go before second
	static bool CompareEditorAsset(EditorAsset*& first, EditorAsset*& second) {
		bool fFolder = first->m_type == EditorAssetType::FOLDER;
		bool sFolder = second->m_type == EditorAssetType::FOLDER;
		if ((fFolder && sFolder) || (!fFolder && !sFolder)) {
			for (int i = 0; i < first->m_name.length() && i < second->m_name.length(); i++) {
				if (tolower(first->m_name[i]) < tolower(second->m_name[i])) return true;
				if (tolower(first->m_name[i]) > tolower(second->m_name[i])) return false;
			}
		}
		return fFolder;
	}

	void Sort() {
		sort(m_assets.begin(), m_assets.end(), CompareEditorAsset);
	}
};

class EditorAssetManager : public Singleton<EditorAssetManager> {
public:
	static StreamedTexture* g_folderTexture;
	EditorAssetManager() : m_assets(new EditorAssetFolder("Assets", nullptr)) {
		m_currentFolder = m_assets;
	}
	EditorAssetFolder* m_assets;
	EditorAssetFolder* m_currentFolder;

	void Initialize() {
		g_folderTexture = GetAssetManager()->Get<StreamedTexture>("FolderIcon");
	}

	void AddAsset(const Path& path, AssetBase* asset) {
		String dir = path.GetDirectory();
		if (!dir._Starts_with("res/Assets")) LOG_ERROR("Invalid asset path");
		String relativePath = dir.substr(10, dir.size());

		auto split = Utils::Split(relativePath, "/");
		EditorAssetFolder* folder = m_assets;

		for (auto& str : split) {
			if (str.empty()) continue;
			bool found = false;
			for (int i = 0; i < folder->m_assets.size(); i++) {
				if (folder->m_assets[i]->m_type == EditorAssetType::FOLDER && folder->m_assets[i]->m_name == str) {
					folder = (EditorAssetFolder*)folder->m_assets[i];
					found = true;
				}
			}
			if (!found) {
				EditorAssetFolder* newFolder = new EditorAssetFolder(str, folder);
				folder->m_assets.push_back(newFolder);
				folder->Sort();
				folder = newFolder;
			}
		}
		folder->m_assets.push_back(new EditorAsset(path.GetFileName(), path.GetFullPath(), asset, EditorAssetType::SPRITE, folder));
		folder->Sort();
	}

	void RemoveAsset(const Path& path) {
		String dir = path.GetDirectory();
		if (!dir._Starts_with("res/Assets")) LOG_ERROR("Invalid asset path");
		String relativePath = dir.substr(10, dir.size());

		auto split = Utils::Split(relativePath, "/");
		EditorAssetFolder* folder = m_assets;
		bool removed = false;
		for (auto& str : split) {
			if (str.empty()) continue;
			for (int i = 0; i < folder->m_assets.size(); i++) {
				if (folder->m_assets[i]->m_type == EditorAssetType::FOLDER && folder->m_assets[i]->m_name == str) {
					folder = (EditorAssetFolder*)folder->m_assets[i];
				}
			}
		}

		for (int i = 0; i < folder->m_assets.size(); i++) {
			if (folder->m_assets[i]->m_fullName == path.GetFullPath()) {
				delete folder->m_assets[i];
				Utils::RemoveFromVector(folder->m_assets, folder->m_assets[i]);
				removed = true;
				break;
			}
		}
		if (!removed) {
			LOG_WARN("[~yAssets~x] failed to remove ~1%s~x from editor asset manager", path.GetFullPath().c_str());
		} else {
			if (folder->m_assets.size() == 0) {
				EditorAssetFolder* parent = (EditorAssetFolder*)folder->m_parent;
				delete folder;
				Utils::RemoveFromVector(parent->m_assets, (EditorAsset*)folder);
			}
		}
	}

	void OnImGui();
};

inline EditorAssetManager* GetEditorAssetManager() { return EditorAssetManager::GetInstance(); }
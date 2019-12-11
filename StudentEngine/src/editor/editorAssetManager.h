#pragma once

enum class EditorAssetType {
	FOLDER,
	SPRITE,
	ANIMATION
};

class EditorAsset : public InspectorDrawable {
public:
	String m_name;
	String m_fullName;
	AssetBase* m_asset;
	EditorAssetType m_type;
	EditorAsset* m_parent;

	EditorAsset(const String& name, const String& fullName, AssetBase* asset, EditorAssetType type, EditorAsset* parent) : m_name(name), m_fullName(fullName), m_asset(asset), m_type(type), m_parent(parent) {}

	virtual StreamedTexture* GetTexture();

	//Op het moment is een asset een folder OF een sprite dus we doen het even zo.
	void InspectorDraw() override {
		InspectorDrawer::Text("Name", m_name.c_str());

		static String_t dropDown[] = { "LINEAR", "NEAREST" };
		TextureParameters params = GetTexture()->GetTexture()->GetTextureParams();
		int type = params.GetFilterType();
		if (InspectorDrawer::Combo("Texture filtering", &type, dropDown, (int)NUMOF(dropDown))) {
			params.SetFilter(TextureFilter(type));
			GetTexture()->GetTexture()->SetTextureParameters(params);
		}
		ImGui::Separator();
	}

	virtual void Clear() {}
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

	void Clear() override {
		for (auto& asset : m_assets) {
			asset->Clear();
			delete asset;
		}
	}

	void InspectorDraw() override {
	}
};

class EditorAssetManager {
public:
	static StreamedTexture* g_folderTexture;
	EditorAssetManager() : m_assets(new EditorAssetFolder("Assets", nullptr)) {
		m_currentFolder = m_assets;
	}
	~EditorAssetManager();
	EditorAssetFolder* m_assets;
	EditorAssetFolder* m_currentFolder;

	void Initialize();
	void AddAsset(const Path& path, AssetBase* asset);
	void RemoveAsset(const Path& path);
	void OnImGui();
};
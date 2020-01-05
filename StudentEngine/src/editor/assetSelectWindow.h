#pragma once
class AssetSelectWindow : public Singleton<AssetSelectWindow>
{
private:
	bool m_windowState;
	static vector<AssetBase*> m_assetList;
	static vector<AssetBase*> m_searchResults;
	function<void(AssetBase*)> m_callback;

	static int UpdateResults(ImGuiTextEditCallbackData* inputData = nullptr);
public:
	AssetSelectWindow();
	void OnImGui();
	void Open();
	void Close();

	template <typename T>
	void Prepare(function<void(AssetBase*)> callback) {
		m_assetList.clear();
		m_assetList = GetAssetManager()->GetAllOfType<T>();
		m_searchResults = m_assetList;
		m_callback = callback;
	}

	void PrepareValidTextures(const char* validTextureKey, function<void(AssetBase*)> callback) {
		m_assetList.clear();
		vector<const char*> validTextureList = GameObject::GetValidTextures(validTextureKey);
		for (const char* asset : validTextureList) {
			m_assetList.push_back((AssetBase*)GetAssetManager()->Get<StreamedTexture*>(asset));
		}
		m_searchResults = m_assetList;
		m_callback = callback;
	}
};

inline AssetSelectWindow* GetAssetSelect() {
	return AssetSelectWindow::GetInstance();
}
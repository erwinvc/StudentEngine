#pragma once
class AssetSelectWindow : public Singleton<AssetSelectWindow>
{
private:
	bool m_windowState;
	static vector<AssetBase*> m_assetList;
	static vector<AssetBase*> m_searchResults;
	void (*m_callback)(AssetBase*);

	static int UpdateResults(ImGuiTextEditCallbackData* inputData = nullptr);
public:
	AssetSelectWindow();
	void OnImGui();
	void Open();
	void Close();

	template <typename T>
	void Prepare(void(*callback)(AssetBase*)) {
		m_assetList = GetAssetManager()->GetAllOfType<T>();
		m_searchResults = m_assetList;
		m_callback = callback;
	}
};

inline AssetSelectWindow* GetAssetSelect() {
	return AssetSelectWindow::GetInstance();
}
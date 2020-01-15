#pragma once

class EditState : public BaseState {
private:
	EditorWindow* m_window;
	//EditorAssetManager* m_assetManager;

	GameObject* m_sample;

public:
	StreamedTexture* g_logo;
	StreamedTexture* g_buttonGizmo;
	StreamedTexture* g_arrowGizmo;
	StreamedTexture* g_squareGizmo;

	EditState();
	~EditState();
	void Initialize() override;
	void Update(const TimeStep& time) override;
	void Draw(RenderingPipeline* pipeline) override;
	void PostDraw(RenderingPipeline* pipeline) override;
	void PostImGuiDraw(RenderingPipeline* pipeline) override;
	void EnterState() override;
	void ExitState() override;
	void OnImGui() override;
	void ResetScene();

	void EditorControls(const TimeStep& time);
	GameObject* AddEditorObject(Vector2 pos, EditorObjectType type);
	inline EditorWindow* GetWindow() { return m_window; }
	//inline EditorAssetManager* GetEditorAssetManager() { return m_assetManager; }
};

EditState* GetEditor();
inline EditorWindow* GetEditorWindow() { return GetEditor()->GetWindow(); }
//inline EditorAssetManager* GetEditorAssetManager() { return GetEditor()->GetEditorAssetManager(); }
#pragma once

class ShaderManager : public Singleton<ShaderManager> {
private:
	map<String, AssetRef<Shader>> m_shaders;
	vector<AssetRef<Shader>> m_shadersVector;
	ShaderManager() {}
	~ShaderManager() {}
	friend Singleton;

public:

	AssetRef<Shader> Create(const String& name, const Path& file, bool hasGeometry = false, bool hasTessellation = false) {
		if (m_shaders[name]) {
			LOG("[~bShaders~x] ~1%s ~rshader has already been created", name.c_str());
			return m_shaders[name];
		}
		AssetRef<Shader> shader = new Shader(name, file, hasGeometry, hasTessellation);
		m_shaders[name] = shader;
		m_shadersVector.push_back(shader);
		return shader;
	}

	AssetRef<Shader> Get(const String& name) {
		return m_shaders[name];
	}

	//void ReloadShaderByFileName(const Tracker& file) {
	//	for (int i = 0; i < m_shadersVector.size(); i++) {
	//		if (m_shadersVector[i]->m_path.GetFullPath().compare(file.m_path.GetFullPath()) == 0) {
	//			m_shadersVector[i]->Reload();
	//		}
	//	}
	//}
	void OnImGui() {
		if (ImGui::BeginTabItem("Shaders")) {
			static int currentlySelectedShader = 0;
			if (ImGui::BeginCombo("Shader", m_shadersVector[currentlySelectedShader]->m_name.c_str())) {
				for (int n = 0; n < m_shadersVector.size(); n++) {
					bool is_selected = (currentlySelectedShader == n);
					if (ImGui::Selectable(m_shadersVector[n]->m_name.c_str(), is_selected))
						currentlySelectedShader = n;
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Separator();
			m_shadersVector[currentlySelectedShader]->OnImGUI();
			ImGui::EndTabItem();
		}
	}

	void Cleanup() {
		for (auto& shader : m_shadersVector) {
			delete shader;
		}
		m_shaders.clear();
		m_shadersVector.clear();
	}
};

static ShaderManager* GetShaderManager() { return ShaderManager::GetInstance(); }
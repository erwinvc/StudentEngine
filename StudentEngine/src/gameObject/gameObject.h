#pragma once

class GameObject : public InspectorDrawable {
private:
	GameObject* m_parent = NULL;
	vector<GameObject*> m_children;
	PhysicsObject m_physicsObject;

public:
	String m_name;
	Transform m_transform;
	Sprite m_sprite;

	GameObject(const String& name) : m_name(name), m_physicsObject(PhysicsObject(this)) {
		m_transform.m_gameObject = this;
	}
	GameObject() : m_name(""), m_physicsObject(PhysicsObject(this)) {
		m_transform.m_gameObject = this;
	}
	~GameObject() {
	}

	virtual GameObject* Copy() {
		return new GameObject(*this);
	}

	void AddChild(GameObject* gameObject) {
		gameObject->SetParent(this);
		m_children.push_back(gameObject);
	}

	void SetParent(GameObject* gameObject) {
		m_parent = gameObject;
	}

	virtual void Update(const TimeStep& time) {
	}

	void Draw(RenderingPipeline* pipeline) {
		pipeline->Rect(m_transform.m_position.x, m_transform.m_position.y, m_transform.m_size.x, m_transform.m_size.y, m_transform.m_rotation, m_sprite.m_color, m_sprite.m_singleFrameTexture ? m_sprite.m_singleFrameTexture : m_sprite.m_texture);
		if (m_sprite.m_singleFrameTexture) m_sprite.m_singleFrameTexture = nullptr;
	}

	#pragma region ChainFunctions
	GameObject& SetPosition(Vector2 position) {
		m_transform.m_position = position;
		return *this;
	}

	GameObject& SetSize(Vector2 size) {
		m_transform.m_size = size;
		return *this;
	}

	GameObject& SetRotation(float rotation) {
		m_transform.m_rotation = rotation;;
		return *this;
	}

	GameObject& SetTexture(StreamedTexture* texture) {
		m_sprite.m_texture = texture;
		return *this;
	}

	GameObject& SetColor(Color color) {
		m_sprite.m_color = color;
		return *this;
	}

	void SetChildren(vector<GameObject*> children) {
		m_children = children;
	}

	GameObject* GetChildAt(int index) {
		return m_children[index];
	}

	vector<GameObject*> GetChildren() {
		return m_children;
	}

	bool HasParent() {
		return m_parent != NULL;
	}

	GameObject* GetParent() {
		return m_parent;
	}

	bool ContainsChild(GameObject* child) {
		bool foundChild = false;
		for (size_t i = 0; i < m_children.size(); i++) {
			foundChild = m_children[i] == child;
		}

		return foundChild;
	}

	void RemoveChild(GameObject* child) {
		m_children.erase(remove(m_children.begin(), m_children.end(), child), m_children.end());
	}

	void InspectorDraw() override {
		InspectorDrawer::EditText("Name", m_name);
		m_transform.InspectorDraw();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Name");
		float width = ImGui::GetContentRegionAvail().x;
		ImGui::SameLine(width - ImGui::CalcTextSize(m_name.c_str(), NULL, true).x);
		ImGui::LabelText("##hideName", m_name.c_str());

		m_sprite.InspectorDraw();
	}

	#pragma endregion
};
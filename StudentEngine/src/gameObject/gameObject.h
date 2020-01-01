#pragma once

static bool nullfunc(GameObject* ths, GameObject* gameObject) { return true; }

class GameObject : public InspectorDrawable {
private:
	GameObject* m_parent = NULL;
	vector<GameObject*> m_children;
	function<bool(GameObject*, GameObject*)> m_onCollisionCallback;

	bool OnCollision(GameObject* other) {
		return m_onCollisionCallback(this, other);
	}

	friend PhysicsObject;
public:
	bool m_destroyNextFrame = false;
	String m_name;
	Transform m_transform;
	PhysicsObject m_physicsObject;
	Sprite m_sprite;

	GameObject(const String& name, bool dynamic = false) : m_name(name), m_onCollisionCallback(nullfunc), m_transform(Transform(this)), m_physicsObject(PhysicsObject(this, dynamic)), m_sprite(Sprite()) {}
	GameObject() : m_name(""), m_onCollisionCallback(nullfunc), m_transform(Transform(this)), m_physicsObject(PhysicsObject(this, false)), m_sprite(Sprite()) {}
	GameObject(const GameObject& other) : m_name(other.m_name), m_onCollisionCallback(other.m_onCollisionCallback), m_transform(Transform(other.m_transform, this)), m_physicsObject(PhysicsObject(other.m_physicsObject, this)), m_sprite(other.m_sprite) {}

	virtual GameObject* Copy() {
		return new GameObject(*this);
	}

	~GameObject() {
	}

	void AddChild(GameObject* gameObject) {
		gameObject->SetParent(this);
		m_children.push_back(gameObject);
	}

	void SetParent(GameObject* gameObject) {
		m_parent = gameObject;
	}

	virtual void Update(const TimeStep& time) {
		m_physicsObject.Update(time);
	}

	void Draw(RenderingPipeline* pipeline) {
		m_sprite.Draw(pipeline, m_transform);
	}

	void Destroy() {
		m_destroyNextFrame = true;
		//GetActiveScene()->DeleteGameObject(this);
	}

	template<typename T>
	bool IsOfType() {
		return dynamic_cast<const T*>(this) != nullptr;
	}

	#pragma region ChainFunctions
	GameObject& SetOnCollision(function<bool(GameObject*, GameObject*)> func) {
		m_onCollisionCallback = func;
		return *this;
	}

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

	GameObject& Set9Slice() {
		m_sprite.m_9Slice = true;
		return *this;
	}

	GameObject& SetAtlasValues(int numberOfRows, int frameCount, float frameTime, int frameOffset = 0) {
		m_sprite.m_numberOfRows = numberOfRows;
		m_sprite.m_frameCount = frameCount;
		m_sprite.m_frameTime = frameTime;
		m_sprite.m_frameOffset = frameOffset;
		ASSERT(m_sprite.m_frameOffset + m_sprite.m_frameCount < m_sprite.m_numberOfRows * m_sprite.m_numberOfRows, "Sprite offset is bigger than the total amount of sprites");
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
#pragma once

class GameObject {
private:
	GameObject* m_parent;
	vector<GameObject*> m_children;

public:

	String m_name;
	Transform m_transform;
	Sprite m_sprite;

	GameObject(Vector2 pos, Vector2 size, Texture* texture) {
		m_transform.m_position = pos;
		m_transform.m_size = size;
		m_sprite.m_texture = texture;
	}

	void AddChild(GameObject* gameObject) {
		gameObject->SetParent(this);
		m_children.push_back(gameObject);
	}

	void SetParent(GameObject* gameObject) {
		m_parent = gameObject;
	}

	GameObject* GetParent() { return m_parent; }

	void Draw(RenderingPipeline* pipeline) {
		pipeline->Rect(m_transform.m_position.x, m_transform.m_position.y, m_transform.m_size.x, m_transform.m_size.y, m_sprite.m_color, m_sprite.m_texture);
	}

	void EditorDraw(RenderingPipeline* pipeline, Color& color) {
		pipeline->Line(m_transform.XMin() - 1, m_transform.YMin() - 1, m_transform.XMin() - 1, m_transform.YMax() + 1, color, 2);
		pipeline->Line(m_transform.XMax() + 1, m_transform.YMin() - 1, m_transform.XMax() + 1, m_transform.YMax() + 1, color, 2);
																											
		pipeline->Line(m_transform.XMin() - 1, m_transform.YMin() - 1, m_transform.XMax() + 1, m_transform.YMin() - 1, color, 2);
		pipeline->Line(m_transform.XMin() - 1, m_transform.YMax() + 1, m_transform.XMax() + 1, m_transform.YMax() + 1, color, 2);
	}
};
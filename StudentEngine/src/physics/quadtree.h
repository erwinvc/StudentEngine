#pragma once

class QuadTreeNode {
private:
	const int MAX_OBJECTS = 3;
	const int MAX_LEVELS = 8;
	int m_level;
	bool m_isLeaf = true;
	vector<Transform*> m_objects;
	Rectangle m_bounds;
	QuadTreeNode* m_nodes[4] = { nullptr };
	QuadTreeNode* m_parent;

	void Subdivide() {
		m_isLeaf = false;
		int halfWidth = (int)m_bounds.w / 2;
		int halfHeight = (int)m_bounds.h / 2;
		int quarterWidth = (int)m_bounds.w / 4;
		int quarterHeight = (int)m_bounds.h / 4;
		int x = (int)m_bounds.x;
		int y = (int)m_bounds.y;

		m_nodes[0] = new QuadTreeNode(m_level + 1, Rectangle(x + quarterWidth, y - quarterHeight, halfWidth, halfHeight), this);
		m_nodes[1] = new QuadTreeNode(m_level + 1, Rectangle(x - quarterWidth, y - quarterHeight, halfWidth, halfHeight), this);
		m_nodes[2] = new QuadTreeNode(m_level + 1, Rectangle(x - quarterWidth, y + quarterHeight, halfWidth, halfHeight), this);
		m_nodes[3] = new QuadTreeNode(m_level + 1, Rectangle(x + quarterWidth, y + quarterHeight, halfWidth, halfHeight), this);
	}

public:
	QuadTreeNode(int level, Rectangle bounds, QuadTreeNode* parent) : m_level(level), m_bounds(bounds), m_parent(parent) {
	}
	~QuadTreeNode() {
		for (int i = 0; i < 4; i++) {
			if (m_nodes[i]) delete m_nodes[i];
		}
	}

	void Draw(RenderingPipeline* pipeline);

	int GetIndex(Rectangle pRect) {
		int index = -1;
		float verticalMidpoint = m_bounds.x;
		float horizontalMidpoint = m_bounds.y;

		float precisionOffset = 4.0f;
		if (pRect.XMax() + precisionOffset < verticalMidpoint) { //Is left
			if (pRect.YMax() + precisionOffset < horizontalMidpoint) index = 1;
			else if (pRect.YMin() - precisionOffset > horizontalMidpoint) index = 2;
			else index = 4;
		} else if (pRect.XMin() - precisionOffset > verticalMidpoint) { //Is right
			if (pRect.YMax() + precisionOffset < horizontalMidpoint) index = 0;
			else if (pRect.YMin() - precisionOffset > horizontalMidpoint) index = 3;
			else index = 5;
		} else { //Is in middle
			if (pRect.YMax() + precisionOffset < horizontalMidpoint) index = 6;
			else if (pRect.YMin() - precisionOffset > horizontalMidpoint) index = 7;
		}
		return index;
	}

	void Insert(Transform* transform) {
		if (!m_isLeaf) {
			Rectangle rect = transform->AsRectangle();
			int index = GetIndex(rect);
			if (Math::Within(index, 0, 3)) {
				m_nodes[index]->Insert(transform);
				return;
			}
		}
		m_objects.push_back(transform);
		if (m_objects.size() > MAX_OBJECTS && m_level < MAX_LEVELS) {
			if (m_isLeaf) Subdivide();
			int i = 0;
			auto it = m_objects.begin();
			while (it != m_objects.end()) {
				int index = GetIndex((*it)->AsRectangle());
				if (Math::Within(index, 0, 3)) {
					m_nodes[index]->Insert(*it);
					it = m_objects.erase(it);
				} else it++;
			}
		}
	}
};

class Quadtree {
private:
	QuadTreeNode* m_baseNode;

public:
	Quadtree() {}
	void Initialize(int width, int height) {
		m_baseNode = new QuadTreeNode(0, Rectangle(0, 0, width, height), nullptr);
	}

	void Insert(Transform* transform) {
		m_baseNode->Insert(transform);
	}
	void Draw(RenderingPipeline* pipeline) {
		m_baseNode->Draw(pipeline);
	}
};
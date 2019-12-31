#include "stdafx.h"

void QuadTreeNode::Draw(RenderingPipeline* pipeline) {
	for (int i = 0; i < 4; i++) {
		if (m_nodes[i]) m_nodes[i]->Draw(pipeline);
	}
	if (GetEditorScene()->GetCursorWorldPosition().Within(m_bounds)) {
		pipeline->Rect(m_bounds, 0, Color(1.0f, 1.0f, 1.0f, 0.5f));
	} else {
		pipeline->LineRect(m_bounds, Color(1.0f, 1.0f, 1.0f, 0.5f));
	}
	for (int i = 0; i < m_objects.size(); i++) {
		pipeline->Rect(m_objects[i]->GetGameObject()->m_transform.m_position.x, m_objects[i]->GetGameObject()->m_transform.m_position.y, 10, 10, 0, Color(1.0f, 0.0f, 0.0f, 0.5f));
	}
}
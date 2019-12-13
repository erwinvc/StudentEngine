#include "stdafx.h"

float EditorGameObject::g_minObjectSize = 15;
float EditorGameObject::g_buttonSize = 18;
float EditorGameObject::g_outlineColorValue = 0;
int EditorGameObject::g_selectedButton = -1;
int EditorGameObject::g_selectedArrow = -1;
Color EditorGameObject::g_outlineColor;
bool EditorGameObject::g_draggingCorner = false;
bool EditorGameObject::g_moving = false;
Vector2 EditorGameObject::g_draggingSizeBackup;
Vector2 EditorGameObject::g_mousePositionBackup;
Vector2 EditorGameObject::g_draggingPositionBackup;
Vector2 EditorGameObject::g_movingOffset;

void EditorGameObject::Draw(RenderingPipeline* pipeline, GameObject* gameObject) {
	float zoom = GetCamera()->GetZoom();
	Transform& transform = gameObject->m_transform;
	static float lineSize = 0.5f;
	static float halfLineSize = lineSize / 2;

	pipeline->LineRect(transform.AsRectangle(), g_outlineColor, lineSize);

	pipeline->Rect(transform.XMin(), transform.YMax(), g_buttonSize * zoom, g_buttonSize * zoom, 0, g_selectedButton == 0 ? Color(10.0f, 0.85f, 0.0f) : Color::White(), GetEditor()->g_buttonGizmo);
	pipeline->Rect(transform.XMax(), transform.YMax(), g_buttonSize * zoom, g_buttonSize * zoom, 0, g_selectedButton == 1 ? Color(10.0f, 0.85f, 0.0f) : Color::White(), GetEditor()->g_buttonGizmo);
	pipeline->Rect(transform.XMin(), transform.YMin(), g_buttonSize * zoom, g_buttonSize * zoom, 0, g_selectedButton == 2 ? Color(10.0f, 0.85f, 0.0f) : Color::White(), GetEditor()->g_buttonGizmo);
	pipeline->Rect(transform.XMax(), transform.YMin(), g_buttonSize * zoom, g_buttonSize * zoom, 0, g_selectedButton == 3 ? Color(10.0f, 0.85f, 0.0f) : Color::White(), GetEditor()->g_buttonGizmo);

	pipeline->Rect(transform.m_position.x, transform.m_position.y + 40 * zoom, 16 * zoom, 80 * zoom, 0, g_selectedArrow == 0 ? Color(10.0f, 0.85f, 0.0f) : Color::Red(), GetEditor()->g_arrowGizmo);
	pipeline->Rect(transform.m_position.x + 40 * zoom, transform.m_position.y, 16 * zoom, 80 * zoom, -Math::HALF_PI, g_selectedArrow == 1 ? Color(10.0f, 0.85f, 0.0f) : Color::Green(), GetEditor()->g_arrowGizmo);
	pipeline->Rect(transform.m_position.x + 20 * zoom, transform.m_position.y + 20 * zoom, 32 * zoom, 32 * zoom, 0, g_selectedArrow == 2 ? Color(2.0f, 1.2f, 0.0f) : Color::White(), GetEditor()->g_squareGizmo);
}

bool EditorGameObject::Update(GameObject* gameObject, const TimeStep& time, Vector2 mousePosition) {
	float zoom = GetCamera()->GetZoom();
	Transform& transform = gameObject->m_transform;

	g_outlineColorValue += time.GetSeconds() * 2.0f;
	g_outlineColor = Color::Mix(Color(1.0f, 0.4f, 0.0f), Color(0.5f, 0.2f, 0.0f), Math::Map(Math::Sin(g_outlineColorValue), -1.0f, 1.0f, 0.0f, 1.0f));

	//Moving
	if (g_moving && !ButtonDown(VK_MOUSE_LEFT)) {
		g_moving = false;
		Undo::FinishRecording();
	}
	if (g_moving) {
		if (g_selectedArrow == 0) {
			if (KeyDown(LCTRL)) transform.m_position.y = Math::RoundToNumber(mousePosition.y + g_movingOffset.y, 100.0f);
			else transform.m_position.y = mousePosition.y + g_movingOffset.y;
		}
		if (g_selectedArrow == 1) {
			if (KeyDown(LCTRL)) transform.m_position.x = Math::RoundToNumber(mousePosition.x + g_movingOffset.x, 100.0f);
			else transform.m_position.x = mousePosition.x + g_movingOffset.x;
		}
		if (g_selectedArrow == 2) {
			if (KeyDown(LCTRL)) {
				transform.m_position.x = Math::RoundToNumber(mousePosition.x + g_movingOffset.x, 100.0f);
				transform.m_position.y = Math::RoundToNumber(mousePosition.y + g_movingOffset.y, 100.0f);
			} else {
				transform.m_position.y = mousePosition.y + g_movingOffset.y;
				transform.m_position.x = mousePosition.x + g_movingOffset.x;
			}
		}
		return true;
	}

	g_selectedArrow = -1;
	if (Math::Within(mousePosition.x, transform.m_position.x + 4 * zoom, transform.m_position.x + 36 * zoom) && Math::Within(mousePosition.y, transform.m_position.y + 4 * zoom, transform.m_position.y + 36 * zoom))g_selectedArrow = 2;
	else if (Math::Within(mousePosition.x, transform.m_position.x - 8 * zoom, transform.m_position.x + 8 * zoom) && Math::Within(mousePosition.y, transform.m_position.y, transform.m_position.y + 80 * zoom))g_selectedArrow = 0;
	else if (Math::Within(mousePosition.x, transform.m_position.x, transform.m_position.x + 80 * zoom) && Math::Within(mousePosition.y, transform.m_position.y - 8 * zoom, transform.m_position.y + 8 * zoom))g_selectedArrow = 1;

	if (g_selectedArrow != -1 && ButtonJustDown(VK_MOUSE_LEFT)) {
		g_movingOffset = transform.m_position - mousePosition;
		g_moving = true;
		Undo::Record(gameObject);
	}

	//Resizing
	if (g_draggingCorner && !ButtonDown(VK_MOUSE_LEFT)) {
		g_draggingCorner = false;
		Undo::FinishRecording();
	}
	if (g_draggingCorner) {
		if (KeyJustDown(LALT) || KeyJustUp(LALT)) transform.m_position = g_draggingPositionBackup;
		if (!KeyDown(LALT)) {
			Vector2 scaled = (g_selectedButton / 2 == 0 ? mousePosition - g_mousePositionBackup : g_mousePositionBackup - mousePosition);
			Vector2 mouseMovedDistance = mousePosition - g_mousePositionBackup;

			if (g_selectedButton == 0 || g_selectedButton == 3) scaled.x *= -1;

			if (KeyDown(LCTRL)) transform.m_size = g_draggingSizeBackup + Math::RoundToNumber(scaled, Vector2(50, 50));
			else transform.m_size = g_draggingSizeBackup + scaled;

			if (transform.m_size.x < g_minObjectSize) transform.m_size.x = g_minObjectSize;
			if (transform.m_size.y < g_minObjectSize) transform.m_size.y = g_minObjectSize;

			switch (g_selectedButton) {
				case 0: {
					if (mouseMovedDistance.x > g_draggingSizeBackup.x - g_minObjectSize) mouseMovedDistance.x = g_draggingSizeBackup.x - g_minObjectSize;
					if (mouseMovedDistance.y < -g_draggingSizeBackup.y + g_minObjectSize) mouseMovedDistance.y = -g_draggingSizeBackup.y + g_minObjectSize;
				} break;
				case 1: {
					if (mouseMovedDistance.x < -g_draggingSizeBackup.x + g_minObjectSize) mouseMovedDistance.x = -g_draggingSizeBackup.x + g_minObjectSize;
					if (mouseMovedDistance.y < -g_draggingSizeBackup.y + g_minObjectSize) mouseMovedDistance.y = -g_draggingSizeBackup.y + g_minObjectSize;
				} break;
				case 2: {
					if (mouseMovedDistance.x > g_draggingSizeBackup.x - g_minObjectSize) mouseMovedDistance.x = g_draggingSizeBackup.x - g_minObjectSize;
					if (mouseMovedDistance.y > g_draggingSizeBackup.y - g_minObjectSize) mouseMovedDistance.y = g_draggingSizeBackup.y - g_minObjectSize;
				} break;
				case 3: {
					if (mouseMovedDistance.x < -g_draggingSizeBackup.x + g_minObjectSize) mouseMovedDistance.x = -g_draggingSizeBackup.x + g_minObjectSize;
					if (mouseMovedDistance.y > g_draggingSizeBackup.y - g_minObjectSize) mouseMovedDistance.y = g_draggingSizeBackup.y - g_minObjectSize;
				} break;
			}
			if (KeyDown(LCTRL)) {
				transform.m_position = g_draggingPositionBackup + Math::RoundToNumber(mouseMovedDistance, Vector2(50, 50)) / 2.0f;
			} else {
				transform.m_position = g_draggingPositionBackup + mouseMovedDistance / 2.0f;

			}
		} else {
			Vector2 scaled = (g_selectedButton / 2 == 0 ? mousePosition - transform.m_position : transform.m_position - mousePosition) * 2;
			if (g_selectedButton == 0 || g_selectedButton == 3) {
				if (scaled.x > g_minObjectSize) scaled.x = g_minObjectSize;
				if (scaled.y < g_minObjectSize) scaled.y = g_minObjectSize;
			} else {
				if (scaled.x < g_minObjectSize) scaled.x = g_minObjectSize;
				if (scaled.y < g_minObjectSize) scaled.y = g_minObjectSize;
			}

			if (KeyDown(LCTRL)) {
				transform.m_size.x = Math::RoundToNumber(Math::Abs(scaled.x), 100.0f);
				transform.m_size.y = Math::RoundToNumber(Math::Abs(scaled.y), 100.0f);
			} else {
				transform.m_size.x = Math::Abs(scaled.x);
				transform.m_size.y = Math::Abs(scaled.y);
			}
		}
		return true;
	}

	g_selectedButton = -1;
	float size = ((g_buttonSize * zoom) * (g_buttonSize * zoom)) * 0.5f;
	if (mousePosition.SqrDistance(Vector2(gameObject->m_transform.XMin(), transform.YMax())) < size) {
		g_selectedButton = 0;
	} else if (mousePosition.SqrDistance(Vector2(gameObject->m_transform.XMax(), transform.YMax())) < size) {
		g_selectedButton = 1;
	} else if (mousePosition.SqrDistance(Vector2(gameObject->m_transform.XMin(), transform.YMin())) < size) {
		g_selectedButton = 2;
	} else if (mousePosition.SqrDistance(Vector2(gameObject->m_transform.XMax(), transform.YMin())) < size) {
		g_selectedButton = 3;
	}

	if (g_selectedButton != -1 && ButtonJustDown(VK_MOUSE_LEFT)) {
		g_draggingPositionBackup = transform.m_position;
		g_draggingSizeBackup = transform.m_size;
		g_mousePositionBackup = mousePosition;
		g_draggingCorner = true;
		Undo::Record(gameObject);
	}

	return g_selectedArrow != -1 || g_selectedButton != -1;
}
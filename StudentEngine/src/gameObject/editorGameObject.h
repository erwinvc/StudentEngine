#pragma once


class EditorGameObject {
private:
	static float g_minObjectSize;
	static float g_buttonSize;
	static int g_selectedButton;
	static int g_selectedArrow;
	static float g_outlineColorValue;
	static Color g_outlineColor;
	static bool g_draggingCorner;
	static bool g_moving;
	static Vector2 g_draggingPositionBackup;
	static Vector2 g_draggingSizeBackup;
	static Vector2 g_mousePositionBackup;
	static Vector2 g_movingOffset;

public:
	static void Draw(RenderingPipeline* pipeline, GameObject* gameObject);
	static bool Update(GameObject* gameObject, const TimeStep& time, Vector2 mousePosition);
};
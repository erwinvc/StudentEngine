#pragma once
class ObjectFactory
{
public:
	static int g_ID;
	static GameObject* CreateObject(EditorObjectType objectType, const String& objectName);
	static void InitializeSize(GameObject* gameObject);
};


#pragma once
class ObjectFactory
{
public:
	static GameObject* CreateObject(EditorObjectType objectType, const String& objectName);
};


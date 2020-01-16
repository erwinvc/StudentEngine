#include "stdafx.h"

int ObjectFactory::g_ID = 0;
GameObject* ObjectFactory::CreateObject(EditorObjectType objectType, const String& objectName) {
	GameObject* newObject;

	switch (objectType) {
		case EditorObjectType::GAMEOBJECT:
			newObject = new GameObject(objectName);
			newObject->SetTexture(GetAssetManager()->Get<StreamedTexture>("Logo"));
			break;
		case EditorObjectType::TERRAIN:
			newObject = new TerrainObject(objectName);
			break;
		case EditorObjectType::PLAYER:
			newObject = new PlayerObject(objectName);
			break;
		case EditorObjectType::GOAL:
			newObject = new GoalObject(objectName);
			break;
		case EditorObjectType::PICKUP_COIN:
			newObject = new CoinPickup(objectName);
			break;
		case EditorObjectType::PICKUP_POWERUP:
			newObject = new PowerupPickup(objectName);
			break;
		case EditorObjectType::ENEMY_STATIC:
			newObject = new StaticEnemy(objectName);
			break;
		case EditorObjectType::ENEMY_WALKING:
			newObject = new WalkingEnemy(objectName);
			break;
	}

	newObject->m_id = g_ID++;
	GetEditorScene()->AddGameObject(newObject);
	return newObject;
}

void ObjectFactory::InitializeSize(GameObject* gameObject) {

}
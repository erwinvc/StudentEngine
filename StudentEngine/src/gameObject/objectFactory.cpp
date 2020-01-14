#include "stdafx.h"

GameObject* ObjectFactory::CreateObject(EditorObjectType objectType, const String& objectName) {
	GameObject* newObject;

	switch (objectType) {
	case EditorObjectType::TERRAIN:
		newObject = GetEditorScene()->AddGameObject(new TerrainObject(objectName))
			->Set9Slice(true)
			->SetTexture(GetAssetManager()->Get<StreamedTexture>("9slice"));
		GetEditorScene()->GetHierarchy().ChangeLayer(newObject, "Background");
		break;
	case EditorObjectType::GAMEOBJECT:
		newObject = GetEditorScene()->AddGameObject(new GameObject(objectName, false))
			->SetTexture(GetAssetManager()->Get<StreamedTexture>("Logo"));
		GetEditorScene()->GetHierarchy().ChangeLayer(newObject, "Objects");
		break;
	case EditorObjectType::PLAYER:
		newObject = GetEditorScene()->AddGameObject(new PlayerObject(objectName))
			->SetTexture(GetAssetManager()->Get<StreamedTexture>("GreyCat"));
		GetEditorScene()->GetHierarchy().ChangeLayer(newObject, "Foreground");
		break;
	case EditorObjectType::PICKUP:
		newObject = GetEditorScene()->AddGameObject(new PickupObject(objectName))
			->SetAtlasValues(4, 4, 0.125f)
			->SetOnCollision([](GameObject* self, GameObject* other, CollisionType type) {
			if (other->IsOfType<PlayerObject>()) {
				static_cast<PickupObject*>(self)->OnPickup();
				self->Destroy();
			}
			return true;
				});
		PickupObject* pickup = (PickupObject*)newObject;
		pickup->SetPickupType(PickupType::COIN);
		GetEditorScene()->GetHierarchy().ChangeLayer(newObject, "Pickups");
		break;
	}

	Texture* objectTexture = newObject->m_sprite.m_texture->GetTexture();
	int sizeX, sizeY;
	if (newObject->m_sprite.m_numberOfRows > 1) {
		sizeX = Math::Max(objectTexture->GetWidth() / newObject->m_sprite.m_frameCount, 16);
		sizeY = Math::Max(objectTexture->GetHeight() / newObject->m_sprite.m_frameCount, 16);
	}
	else
	{
		sizeX = Math::Max(objectTexture->GetWidth(), 16);
		sizeY = Math::Max(objectTexture->GetHeight(), 16);
	}
	newObject->SetSize(Vector2(sizeX, sizeY));

	return newObject;
}
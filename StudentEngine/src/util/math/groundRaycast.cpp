#include "stdafx.h"

Vector3 GroundRaycast::GetMousePosition(const Camera* camera) {
	return CalculateMouseRay(camera);
}

Vector3 GroundRaycast::GetScreenPosition(Vector2 position, const Camera* camera) {
	position.x = (position.x * 2.0f) - 1.0f;
	position.y = -1.0f * ((position.y * 2.0f) - 1.0f);
	return CalculateScreenRay(Vector2(position), camera);
}

Vector3 GroundRaycast::CalculateScreenRay(const Vector2& position, const Camera* camera) {
	Vector4 clipCoords = Vector4(position.x, position.y, -1.0f, -1.0f);
	Vector4 eyeCoords = ToEyeCoords(clipCoords, camera);
	Vector3 worldRay = ToWorldCoords(eyeCoords, camera);
	return worldRay;
}
Vector3 GroundRaycast::CalculateMouseRay(const Camera* camera) {
	float mouseX = GetMouse()->GetPosition().x;
	float mouseY = GetMouse()->GetPosition().y;/* -19 * 2;*/
	Vector4 viewport = camera->GetViewport();
	//LOG("%f %f %f %f", viewport.x, viewport.y, viewport.z, viewport.w);
	mouseX = Math::Clamp(mouseX, viewport.x, viewport.x + viewport.z);
	mouseY = Math::Clamp(mouseY, viewport.y, viewport.y + viewport.w);
	mouseX = Math::Map(mouseX, viewport.x, viewport.x + viewport.z, -1.0f, 1.0f);
	mouseY = Math::Map(mouseY, viewport.y, viewport.y + viewport.w, 1.0f, -1.0f);
	Vector4 clipCoords = Vector4(mouseX, mouseY, -1.0f, -1.0f);
	Vector4 eyeCoords = ToEyeCoords(clipCoords, camera);
	Vector3 worldRay = ToWorldCoords(eyeCoords, camera);
	return worldRay;
}

Vector4 GroundRaycast::ToEyeCoords(const Vector4& clipCoords, const Camera* camera) {
	Matrix4 invertedProjection = Matrix4::Invert(camera->GetProjectionMatrix());
	Vector4 eyeCoords = invertedProjection * clipCoords;
	return Vector4{ eyeCoords.x, eyeCoords.y, -1.0f, 0.0f };
}
Vector3 GroundRaycast::ToWorldCoords(const Vector4& eyeCoords, const Camera* camera) {
	Matrix4 invertedView = Matrix4::Invert(camera->GetViewMatrix());
	Vector4 rayWorld = invertedView * eyeCoords;
	Vector3 mouseRay = Vector3(rayWorld.x, rayWorld.y, rayWorld.z);
	return mouseRay.Normalize();
}
Vector3 GroundRaycast::GetGroundPosition(const Camera* camera, const Vector3& ray, float height) {
	Vector3 rayStart = camera->m_position;
	Vector3 rayEnd = camera->m_position + ray;
	Vector3 delta = rayEnd.Subtract(rayStart);
	delta.Normalize();

	Vector3 rayToPlaneDelta = Vector3(0.0f, 0.0f, 1.0f).Subtract(rayStart);

	float ratio = rayToPlaneDelta.Dot(Vector3::ZAxis());
	Vector3& proj = Vector3::ZAxis().Multiply(ratio);

	auto wp = rayToPlaneDelta.Dot(Vector3::ZAxis());
	auto vp = delta.Dot(Vector3::ZAxis());
	auto k = wp / vp;
	Vector3 toRet = delta.Multiply(-k).Add(rayStart);
	toRet.x += camera->GetViewport().z;
	toRet.y += camera->GetViewport().w;
	return toRet;
}
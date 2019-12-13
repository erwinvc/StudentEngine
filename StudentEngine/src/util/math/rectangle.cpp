#include "stdafx.h"

//Checks if this rect is within other rect
bool Rectangle::Within(Rectangle& other) {
	bool withinX = Math::Within(position.x - size.x / 2, position.x + size.x / 2, other.position.x - other.size.x / 2, other.position.x + other.size.x / 2);
	if (!withinX) return false;
	bool withinY = Math::Within(position.y - size.y / 2, position.y + size.y / 2, other.position.y - other.size.y / 2, other.position.y + other.size.y / 2);
	if (!withinY) return false;
	return true;
}
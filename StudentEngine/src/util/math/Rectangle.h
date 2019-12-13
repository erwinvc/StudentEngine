#pragma once

struct Rectangle {
	union {
		struct {
			Vector2 position;
			Vector2 size;
		};
		struct {
			float x, y, w, h;
		};
	};

	Rectangle() :x(0), y(0), w(0), h(0) {}
	Rectangle(float _x, float _y, float _w, float _h) : x(_x), y(_y), w(_w), h(_h) {}
	Rectangle(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h) {}
	Rectangle(Vector2& _position, Vector2& _size) : position(_position), size(_size) {}

	Rectangle GetCornerPositions() const {
		return { position - size / 2, position + size / 2 };
	}

	bool Within(Rectangle& other);

	float XMin() { return position.x - size.x / 2; }
	float XMax() { return position.x + size.x / 2; }
	float YMin() { return position.y - size.y / 2; }
	float YMax() { return position.y + size.y / 2; }
};
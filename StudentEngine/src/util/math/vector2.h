#pragma once

struct Vector3;
struct Vector2I;
struct Rectangle;

struct Vector2 {
	float x, y;

	Vector2();
	Vector2(float scalar);
	Vector2(int scalar);
	Vector2(float x, float y);
	Vector2(int x, int y);
	Vector2(const aiVector2D& vec) : x(vec.x), y(vec.y) {}
	Vector2(const Vector3& vector);

	Vector2& Add(const Vector2& other);
	Vector2& Add(const Vector2I& other);
	Vector2& Subtract(const Vector2& other);
	Vector2& Subtract(const Vector2I& other);
	Vector2& Multiply(const Vector2& other);
	Vector2& Multiply(const Vector2I& other);
	Vector2& Divide(const Vector2& other);
	Vector2& Divide(const Vector2I& other);
	Vector2 Pow(float pow);
	
	Vector2& Add(float value);
	Vector2& Subtract(float value);
	Vector2& Multiply(float value);
	Vector2& Divide(float value);

	friend Vector2 operator+(Vector2 left, const Vector2& right);
	friend Vector2 operator-(Vector2 left, const Vector2& right);
	friend Vector2 operator*(Vector2 left, const Vector2& right);
	friend Vector2 operator/(Vector2 left, const Vector2& right);

	friend Vector2 operator+(Vector2 left, float value);
	friend Vector2 operator-(Vector2 left, float value);
	friend Vector2 operator*(Vector2 left, float value);
	friend Vector2 operator/(Vector2 left, float value);

	bool operator==(const Vector2& other) const;
	bool operator!=(const Vector2& other) const;

	Vector2& operator+=(const Vector2& other);
	Vector2& operator-=(const Vector2& other);
	Vector2& operator*=(const Vector2& other);
	Vector2& operator/=(const Vector2& other);

	Vector2& operator+=(float value);
	Vector2& operator-=(float value);
	Vector2& operator*=(float value);
	Vector2& operator/=(float value);

	bool operator<(const Vector2& other) const;
	bool operator<=(const Vector2& other) const;
	bool operator>(const Vector2& other) const;
	bool operator>=(const Vector2& other) const;

	float Magnitude() const;
	Vector2& Normalize();
	Vector2 Normalized() const;
	float SqrDistance(const Vector2& other) const;
	float Distance(const Vector2& other) const;
	float Dot(const Vector2& other) const;

	//Checks if point is within rect
	bool Within(Rectangle& other) const;

	friend void to_json(nlohmann::json& j, const Vector2& obj) {
		j = nlohmann::json{ {"x", obj.x}, {"y", obj.y} };
	}

	friend void from_json(const nlohmann::json& j, Vector2& obj) {
		j.at("x").get_to(obj.x);
		j.at("y").get_to(obj.y);
	}
};
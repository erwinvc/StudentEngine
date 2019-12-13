#pragma once

struct Matrix4;

struct Vector4 {
	union
	{
		struct { float x, y, z, w; };
		struct { float x, y, width, height; };
		float values[4];
	};

	Vector4() = default;
	Vector4(float scalar);
	Vector4(float x, float y, float z, float w);
	Vector4(const Vector3& xyz, float w);
	Vector4(const Vector2& v1, const Vector2& v2);

	Vector4& Add(const Vector4& other);
	Vector4& Subtract(const Vector4& other);
	Vector4& Multiply(const Vector4& other);
	Vector4& Divide(const Vector4& other);

	Vector4 Multiply(const Matrix4& transform) const;

	friend Vector4 operator+(Vector4 left, const Vector4& right);
	friend Vector4 operator-(Vector4 left, const Vector4& right);
	friend Vector4 operator*(Vector4 left, const Vector4& right);
	friend Vector4 operator/(Vector4 left, const Vector4& right);

	bool operator==(const Vector4& other);
	bool operator!=(const Vector4& other);

	Vector4& operator+=(const Vector4& other);
	Vector4& operator-=(const Vector4& other);
	Vector4& operator*=(const Vector4& other);
	Vector4& operator/=(const Vector4& other);

	float Dot(const Vector4& other);
};
#pragma once

typedef int8_t		int8;
typedef int16_t		int16;
typedef int32_t		int32;
typedef int64_t		int64;

typedef uint8_t		uint8;
typedef uint16_t	uint16;
typedef uint32_t	uint32;
typedef uint64_t	uint64;

typedef unsigned int uint;
typedef unsigned char byte;
typedef std::string String;
typedef const char* String_t;

enum CollisionType {
	NONE,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

enum class EditorObjectType {
	GAMEOBJECT,
	TERRAIN,
	PLAYER,
	GOAL,
	PICKUP_COIN,
	PICKUP_POWERUP,
	ENEMY_STATIC,
	ENEMY_WALKING
};
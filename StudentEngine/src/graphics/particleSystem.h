#pragma once

#define PARTICLE_COUNT 0x200

class ParticleSystem {
private:
	Vector2 m_positions[PARTICLE_COUNT];
	Vector2 m_velocities[PARTICLE_COUNT];
	Vector2 m_size[PARTICLE_COUNT];
	int m_life[PARTICLE_COUNT];
	int m_index = 0;
public:
};
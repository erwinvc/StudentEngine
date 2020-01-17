#pragma once

#define PARTICLE_COUNT 0x200

class ParticleSystem {
private:
	Vector2 m_position[PARTICLE_COUNT];
	Vector2 m_velocity[PARTICLE_COUNT];
	Vector2 m_size[PARTICLE_COUNT];
	float m_friction[PARTICLE_COUNT];
	float m_gravity[PARTICLE_COUNT];
	Color m_color[PARTICLE_COUNT];
	int m_life[PARTICLE_COUNT];
	int m_sprite[PARTICLE_COUNT];
	Vector3 m_atlasValues[16];
public:
	StreamedTexture* m_texture;

	ParticleSystem() {
		for (int i = 0; i < PARTICLE_COUNT; i++) {
			m_life[i] = 0;
		}

		for (int i = 0; i < 16; i++) {
			float row = 4 - (i / 4) - 1;
			float column = i % 4;
			m_atlasValues[i] = Vector3(4, column / 4, row / 4);
		}

	}
	void Update(const TimeStep& time) {
		for (int i = 0; i < PARTICLE_COUNT; i++) {
			if (m_life[i] > 0) {
				float decay = 1 / (1 + (time * m_friction[i]));
				m_velocity[i] *= decay;
				m_position[i].y -= m_gravity[i] * time;
				m_position[i] += m_velocity[i];
				m_life[i] -= time;
			}
		}
	}

	void Add(float x, float y, float velX, float velY, float gravity, float sizeX, float sizeY, float friction, float life, int sprite, Color& color) {
		for (int i = 0; i < PARTICLE_COUNT; i++) {
			if (m_life[i] <= 0) {
				m_position[i].x = x;
				m_position[i].y = y;
				m_velocity[i].x = velX;
				m_velocity[i].y = velY;
				m_size[i].x = sizeX;
				m_size[i].y = sizeY;
				m_friction[i] = friction;
				m_gravity[i] = gravity;
				m_life[i] = life;
				m_sprite[i] = sprite;
				m_color[i] = color;
				return;
			}
		}
	}

	void Draw(RenderingPipeline* pipeline) {
		for (int i = 0; i < PARTICLE_COUNT; i++) {
			if (m_life[i] > 0) {
				pipeline->Rect(Rectangle(m_position[i].x, m_position[i].y, m_size[i].x, m_size[i].y), 0, m_color[i], m_texture, m_atlasValues[m_sprite[i]]);
			}
		}
	}
};
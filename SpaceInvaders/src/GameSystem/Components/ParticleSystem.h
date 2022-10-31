#pragma once
#include "Components.h"
#include "Transform.h"
#include "../Core/Random/Random.h"

struct Particle
{
	Vec3f position;
	Vec3f velocity;
	Vec4f color;
	float rotation;
	float size;
	float lifetime;
	unsigned int index;
};

struct ParticleEmitter
{
	ParticleEmitter(float _angle, Vec4f _color, float _variance, bool _loop, unsigned int _numParticles, float _lifetime)
	{
		angle = _angle;
		color = _color;
		colorVariation = _variance;
		loop = _loop;
		numParticles = _numParticles;
		lifetime = _lifetime;
		
	}

	ParticleEmitter()
	{
	
	}

	float angle;
	Vec4f color;
	float colorVariation;
	bool loop;
	unsigned int numParticles;
	float lifetime;
	bool initialized = false;
	std::shared_ptr<std::vector<Particle>> particles;
};

class ParticleSystem : public ECSSystem
{
public:
	ParticleSystem()
	{
		auto unlitInstanced = ShaderLoader::Load("resources/shaders/unlit_instanced.glsl");
		material = std::make_shared<Material>(unlitInstanced, true);
		rect = std::make_shared<RectGeometry>(Vec2f(0.2, 0.2));
	}

	Random rng;
	std::shared_ptr<Material> material;
	std::shared_ptr<RectGeometry> rect;

	virtual void Update(float dt) override;
	virtual void OnCreate(EntityID entity) override;

	void UpdateParticle(Particle& particle, Transform& transform, ParticleEmitter& emitter);
};
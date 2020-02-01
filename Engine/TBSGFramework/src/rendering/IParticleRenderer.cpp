#include "rendering/IParticleRenderer.h"

namespace gfx
{
	void IParticleRenderer::UpdateParticleEmitters(float totalTime, float deltaTime)
	{
		for(auto particleEmitterId : uninitializedEmitters) {
			auto& punctualParticleEmitter = particleEmitters[particleEmitterId];
			InitializeParticleEmitter(punctualParticleEmitter.particleEmitter, totalTime, deltaTime);
		}
		uninitializedEmitters.clear();

		for (auto& punctualParticleEmitter : particleEmitters) {
			UpdateParticleEmitters(punctualParticleEmitter.particleEmitter, totalTime, deltaTime);
		}
	}

	ParticleEmitterId IParticleRenderer::AddEmitter(const core::Transform& position, ParticleEmitter&& emitter)
	{
		ParticleEmitterId id = particleEmitters.push_back(PunctualParticleEmitter{ position, emitter });
		uninitializedEmitters.push_back(id);
		return id;
	}

	void IParticleRenderer::RemoveEmitter(ParticleEmitterId id)
	{
		
		auto it = std::find(uninitializedEmitters.begin(), uninitializedEmitters.end(), id);
		if(it != uninitializedEmitters.end()) {
			uninitializedEmitters.erase(it);
		}

		particleEmitters.erase(id);
	}

	bool IParticleRenderer::HasEmitter(ParticleEmitterId id)
	{
		return particleEmitters.contains(id);
	}

	void IParticleRenderer::InitializeParticleEmitter(ParticleEmitter& emitter, float totalTime, float deltaTime)
	{
		UNUSED(deltaTime);

		uint32_t initialParticleAmount = static_cast<uint32_t>(emitter.startParameters.initialParticleAmount.Value());
		emitter._particles.reserve(initialParticleAmount);
		float currentTime = static_cast<float>(totalTime);
		for (uint32_t i = 0; i < initialParticleAmount; ++i) {
			glm::vec3 particlePos{};
			glm::vec3 particleVel{};

			
			
			glm::vec3 relativeOffset = emitter.startParameters.spawnOffset.Value();
			
			particlePos = emitter.particleEmitterPosition + relativeOffset;

			particleVel = emitter.startParameters.initialVelocity.Value();

			
				
				
				
			

			float mass = emitter.startParameters.mass.Value();

			ParticleEmitter::ParticleInstance instance{ particlePos, mass, particleVel, currentTime };
			emitter._particles.push_back(instance);
		}
	}

	void IParticleRenderer::UpdateParticleEmitters(ParticleEmitter& emitter, float totalTime, float deltaTime)
	{
		bool useGravity = emitter.startParameters.useGravity;
		float dt = deltaTime;

		auto& particles = emitter._particles;
		const auto& params = emitter.startParameters;
		for (int i = static_cast<int>(particles.size() - 1); i >= 0; --i) {
			auto& particle = particles[i];

			particle.position += particle.velocity * dt;
			if (useGravity)
				particle.velocity -= glm::vec3(0.f, -9.81f, 0.f) * particle.mass * dt;
			particle.velocity = glm::normalize(particle.velocity) * (glm::length(particle.velocity) + params.velocityChangeOverTime.Value() * dt);

			float lifeTime = params.lifeTime.Value();
			if (lifeTime > 0.0f) {
				if (particle.birthTime + lifeTime <= totalTime) {
					
					std::swap(particle, *--particles.end());
					particles.erase(--particles.end());

					continue;
				}
			}
		}

		
	}

	void IParticleRenderer::SortParticles(Camera& camera)
	{
		using Particle = typename ParticleEmitter::ParticleInstance;

		glm::mat4 vp = camera.get_ProjectionMatrix() * camera.get_ViewMatrix();

		for (auto& punctualEmitter : particleEmitters) {
			std::sort(punctualEmitter.particleEmitter._particles.begin(), punctualEmitter.particleEmitter._particles.end(), [&vp](const Particle& a, const Particle& b) {
				auto posA = vp * glm::vec4{ a.position, 1.0 };
				auto posB = vp * glm::vec4{ b.position, 1.0 };

				return posA.z > posB.z;
			});
		}
	}
}

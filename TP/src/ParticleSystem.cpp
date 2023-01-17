#include <ParticleSystem.h>
#include <cstddef>


namespace OM3D {

ParticleSystem::ParticleSystem(std::vector<Particle> particles)
{
    _particle_buffer_compute = TypedBuffer<shader::Particle>(nullptr, std::max(particles.size(), size_t(1)));
    {
        auto mapping = _particle_buffer_compute.map(AccessType::ReadWrite);
        for(size_t i = 0; i != particles.size(); ++i) {
            const auto& particle = particles[i];
            mapping[i] = {
                particle._position,
                particle._radius,
                particle._velocity,
                particle._age,
                particle._force,
                particle._lifetime,
                particle._color,
            };
        }
    }
    _particle_buffer_render = TypedBuffer<shader::ParticleVertex>(nullptr, std::max(particles.size() * 4, size_t(1)));
    {
        auto mapping = _particle_buffer_render.map(AccessType::ReadWrite);
        for(size_t i = 0; i != particles.size(); ++i) {
            const auto& particle = particles[i];
            mapping[i] = {
                particle._position,
                particle._color,
                glm::vec2(0.0f, 0.0f),
            };
        }
    }
}

}
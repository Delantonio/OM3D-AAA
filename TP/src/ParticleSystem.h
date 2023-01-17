#include <Particle.h>
#include <vector>
#include "TypedBuffer.h"
#include <vector>
#include "glm/ext/matrix_float4x4.hpp"
#include <shader_structs.h>

namespace OM3D {

class ParticleSystem
{
    public:
        ParticleSystem(std::vector<Particle> particles);

        void update(float dt);
        void render() const;

        void add_particle(const Particle& particle);
        void add_particles(const std::vector<Particle>& particles);

        void set_transform(const glm::mat4& tr);
        const glm::mat4& transform() const;

    private:
        OM3D::TypedBuffer<shader::Particle> _particle_buffer_compute;
        OM3D::TypedBuffer<shader::ParticleVertex> _particle_buffer_render;
        glm::mat4 _transform = glm::mat4(1.0f);
};

}

#include <ParticleSystem.h>
#include <cstddef>
#include "Camera.h"
#include "Material.h"


namespace OM3D {

ParticleSystem::ParticleSystem(std::shared_ptr<Program> program_compute, std::shared_ptr<Material> material, const std::vector<Particle> &particles)
{
    _program_compute = std::move(program_compute);
    _render_material = std::move(material);

    map_particles();
    _particles = particles;

    const std::vector<Vertex> ParticleVertex = {
        Vertex(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0),
               glm::vec2(0.0, 0.0)),
        Vertex(glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0),
               glm::vec2(1.0, 0.0)),
        Vertex(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, -1.0),
               glm::vec2(0.0, 1.0)),
        Vertex(glm::vec3(1.0, 1.0, 0.0), glm::vec3(0.0, 0.0, -1.0),
               glm::vec2(1.0, 1.0)),
    };

    const std::vector<u32> ParticleIndices = { 0, 1, 2, 1, 2, 3 };
    _particle_vertex_buffer = TypedBuffer<Vertex>(ParticleVertex);
    _particle_index_buffer = TypedBuffer<u32>(ParticleIndices);
}

void ParticleSystem::map_particles()
{
    // auto _particle_buffer_compute = TypedBuffer<shader::Particle>(nullptr, std::max(_particles.size(), size_t(1)));
    // {
    //     auto mapping = _particle_buffer_compute.map(AccessType::ReadWrite);
    //     for (size_t i = 0; i != _particles.size(); ++i)
    //     {
    //         const auto &particle = _particles[i];
    //         mapping[i] = {
    //             particle._transform, particle._color, particle._velocity,
    //             particle._age,       particle._force, particle._lifetime,
    //             particle._center,
    //             0.0f, // luminosity
    //         };
    //     }
    //     _particle_buffer_compute.bind(BufferUsage::Storage, 1);
    // }
}

void ParticleSystem::update(float dt)
{
    // update in compute
    // _program_compute->bind();
    _program_compute->set_uniform(HASH("dt"), dt);
    glDispatchCompute(128, 1, 1);
}

void ParticleSystem::bind_render() const
{
    _render_material->bind();
}
void ParticleSystem::bind_compute() const
{
    _program_compute->bind();
}

void ParticleSystem::render()
{
    // draw
    // _render_material->bind();
    _render_material->set_cull_mode(CullMode::None);
    _render_material->set_depth_test_mode(DepthTestMode::None);
    _render_material->set_blend_mode(BlendMode::None);

    _particle_vertex_buffer.bind(BufferUsage::Attribute);
    _particle_index_buffer.bind(BufferUsage::Index);

    // Vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), nullptr);
    // Vertex normal
    // glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex),
    //                       reinterpret_cast<void *>(3 * sizeof(float)));
    // Vertex uv
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex),
                          reinterpret_cast<void *>(6 * sizeof(float)));
    // // Tangent / bitangent sign
    // glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Vertex),
    //                       reinterpret_cast<void *>(8 * sizeof(float)));
    // // Vertex color
    // glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertex),
    //                       reinterpret_cast<void *>(12 * sizeof(float)));

    glEnableVertexAttribArray(0);
    // glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    // glEnableVertexAttribArray(3);
    // glEnableVertexAttribArray(4);

    int instance_count = _particles.size();
    glDrawElementsInstanced(
        GL_TRIANGLE_STRIP,
        int(_particle_index_buffer.element_count()),
        GL_UNSIGNED_INT, nullptr, instance_count);
}

} // namespace OM3D
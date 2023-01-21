#include "ParticleSystem.h"
#include <cstddef>
#include <memory>
#include "Camera.h"
#include "Material.h"


namespace OM3D {

ParticleSystem::ParticleSystem(std::shared_ptr<Program> program_compute, std::shared_ptr<Material> material, const std::vector<Particle> &particles)
{
    _program_compute = std::move(program_compute);
    _render_material = std::move(material);

    _particles = particles;

    // Particle Mesh
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

    _particle_buffer_compute = TypedBuffer<shader::Particle>(
        nullptr, std::max(particles.size(), size_t(1)));

    {
        auto mapping = _particle_buffer_compute.map(AccessType::ReadWrite);
        for (size_t i = 0; i != particles.size(); ++i)
        {
            const auto &particle = particles[i];
            mapping[i] = {
                particle._color,      particle._velocity, particle._duration,
                particle._force,      particle._seed,     particle._center,
                particle._luminosity, particle._origin, particle._age,
            };
        }
    }
    _particle_texture = nullptr;
}

void ParticleSystem::texture_from_file(const std::string &texture_path)
{
    Result<TextureData> texture_data = TextureData::from_file(texture_path);
    ALWAYS_ASSERT(texture_data.is_ok, "Unable to load texture from file");

    _particle_texture = std::make_shared<Texture>(std::move(texture_data.value));
    
    _render_material->set_texture(HASH("in_texture"), _particle_texture);
}

void ParticleSystem::update(float dt)
{
    _program_compute->bind();

    _particle_buffer_compute.bind(BufferUsage::Storage, 1);

    _program_compute->set_uniform(HASH("dt"), dt);
    glDispatchCompute(align_up_to(_particles.size(), 8) / 8, 1, 1);
}

void ParticleSystem::render()
{
    // _render_material->bind();

    _particle_buffer_compute.bind(BufferUsage::Storage, 1);
    
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